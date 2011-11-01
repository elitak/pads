(***********************************************************************
*                                                                      *
*              This software is part of the pads package               *
*           Copyright (c) 2005-2007 AT&T Knowledge Ventures            *
*                      and is licensed under the                       *
*                        Common Public License                         *
*                      by AT&T Knowledge Ventures                      *
*                                                                      *
*                A copy of the License is available at                 *
*                    www.padsproj.org/License.html                     *
*                                                                      *
*  This program contains certain software code or other information    *
*  ("AT&T Software") proprietary to AT&T Corp. ("AT&T").  The AT&T     *
*  Software is provided to you "AS IS". YOU ASSUME TOTAL RESPONSIBILITY*
*  AND RISK FOR USE OF THE AT&T SOFTWARE. AT&T DOES NOT MAKE, AND      *
*  EXPRESSLY DISCLAIMS, ANY EXPRESS OR IMPLIED WARRANTIES OF ANY KIND  *
*  WHATSOEVER, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF*
*  MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, WARRANTIES OF  *
*  TITLE OR NON-INFRINGEMENT.  (c) AT&T Corp.  All rights              *
*  reserved.  AT&T is a registered trademark of AT&T Corp.             *
*                                                                      *
*                   Network Services Research Center                   *
*                          AT&T Labs Research                          *
*                           Florham Park NJ                            *
*                                                                      *
*              Kathleen Fisher <kfisher@research.att.com>              *
*                 Robert Gruber <bob.gruber@gmail.com>                 *
*            Yitzhak Mandelbaum <yitzhakm@cs.princeton.edu>            *
*                                                                      *
***********************************************************************)
type svalue = Tokens.svalue
type pos = int
type lexresult = (svalue,pos) Tokens.token
type errWarn = {err: pos*pos*string->unit, warn: pos*pos*string->unit}
type lexarg =  {comLevel : int ref, 
                sourceMap : SourceMap.sourcemap,
		charlist : string list ref,
		stringstart : int ref,  (* start of current string or comment*)
	        errWarn: errWarn}

type arg = lexarg
type ('a,'b) token = ('a,'b) Tokens.token

fun ordof (s, i) = Char.ord (String.sub (s, i))
fun dec (iRef : int ref) = iRef := (!iRef) - 1
fun inc (iRef : int ref) = iRef := (!iRef) + 1
fun chr i = String.str(Char.chr i)
fun ord s = Char.ord(String.sub(s, 0))
fun explode s = CharVector.foldr (fn (c, l) => str c :: l) [] s
fun implode strList = String.concat strList
fun hd [] = (print "c.lex: hd of empty\n";
	     raise Empty)
  | hd (h :: l)	= h

val eof = fn ({comLevel,errWarn,sourceMap,stringstart,charlist}:lexarg) => 
	   let val pos = Int.max(!stringstart+2, SourceMap.currPos sourceMap)
	    in if !comLevel>0 then (#err errWarn) (!stringstart,pos, "unclosed comment" )
		  	      else ();
	       Tokens.EOF(pos,pos)
	   end	
fun addString (charlist,s:string) = charlist := s :: (!charlist)
fun makeString charlist = (implode(rev(!charlist)) before charlist := nil)

fun mkHexInt (s,a,b,errWarn:errWarn)=((case (StringCvt.scanString (IntInf.scan StringCvt.HEX) (String.extract(s,2,NONE))) of
		  SOME i =>  i
		| _ => ((#err errWarn)(a,b,"trouble in parsing int");IntInf.fromInt(0)))
			handle OverFlow => ((#err errWarn)(a,b,"large int const");IntInf.fromInt(0)))

fun mkHexChar (args as (s, a, b, errWarn:errWarn)) : int (* returns a character sized integer *) = 
	let val i = mkHexInt args
            val i = (IntInf.toInt i) 
                    handle OverFlow => ((#err errWarn)(a,b,"large int const");0)
	in
	  if (i>255) then 
	    ((#warn errWarn) (a,b,"overflow in hexadecimal escape sequence");
	    (i mod 256))
	  else
       	    i
        end	

fun mkOctInt (s,a,b,errWarn:errWarn)
	= ((case (StringCvt.scanString (IntInf.scan StringCvt.OCT) s) of
		  SOME i => i
		| _ => ((#err errWarn)(a,b,"trouble in parsing int");IntInf.fromInt(0)))
			handle OverFlow => ((#err errWarn)(a,b,"large int const");IntInf.fromInt(0)))


fun mkOctChar (args as (s, a, b, errWarn:errWarn)) (* returns a character sized integer *) = 
	let val i = mkOctInt args
            val i = (IntInf.toInt i) 
                    handle OverFlow => ((#err errWarn)(a,b,"large int const");0)
	in
	  if (i>255) then 
	    ((#warn errWarn) (a,b,"overflow in octal escape sequence");
	    (i mod 256))
	  else
       	     i
        end	

fun mkInt (s,a,b,errWarn:errWarn) = ((case (StringCvt.scanString (IntInf.scan StringCvt.DEC) s) of
		  SOME i => i
		| _ => ((#err errWarn)(a,b,"trouble in parsing int");IntInf.fromInt(0)))
			handle OverFlow => ((#err errWarn)(a,b,"large int const");IntInf.fromInt(0)))

fun mkRealNum (s,a,b,errWarn:errWarn) = ((case (StringCvt.scanString Real.scan s) of
		   SOME r => r
		 | _ => ((#err errWarn)(a,b,"trouble in parsing real");0.0))
			handle OverFlow => ((#err errWarn)(a,b,"large real const"); 0.0))

val backslasha = 7

fun special_char(c,fst,last,errWarn:errWarn) =
		(case c of
			"\\a" => 7
		      | "\\b" => 8
		      | "\\f" => 12
		      | "\\n" => 10
		      | "\\r" => 13
		      | "\\t" => 9
		      | "\\v" => 11
	              | _ => ordof(c,1)
	                      (* strictly speaking, should only handle
                                \?, \\, \", \', but it is common
                                to simply ignore slash, and just use next char *)
		)


(* Notes on lexer states:
   INITIAL -- predefined start state and the default token state
   S -- inside a string (entered from INTITAL with ")
   C -- inside a comment (entered from INITIAL with /* )
   CC -- inside a C++ style comment
   Pc -- inside a pads comment
 *)


%%

%header (functor TLexFun(structure Tokens : T_TOKENS 
			 structure TTokTable : TTOKENTABLE 
			 sharing TTokTable.Tokens = Tokens));

%arg ({comLevel,errWarn,sourceMap,charlist,stringstart});
%s CC C S PC; 

eol=.*"\n";
newline = ("\010" | "\010\013" | "\013" | "\013\010");
id	= [_A-Za-z][_A-Za-z0-9]*; 
octdigit	= [0-7];
hexdigit	= [0-9a-fA-F];
hexnum	= 0[xX]{hexdigit}+[uUlL]?[uUlL]?; 
octnum	= 0{octdigit}+[uUlL]?[uUlL]?;
decnum	= (0|([1-9][0-9]*))[uUlL]?[uUlL]?;
realnum = (([0-9]+(\.[0-9]+)?)|(\.[0-9]+))([eE][+-]?[0-9]+)?[lL]?;
ws	= ("\012"|[\t\ ])*;

simplecharconst  = '[^\n\\]';
escapecharconst  = '\\[^\n]';

directive = #(.)*\n;

%%

<INITIAL,C>^{ws}{directive}     => (SourceMap.parseDirective sourceMap 
                         (yypos,yytext); continue());
<INITIAL,C>\n		=> (SourceMap.newline sourceMap yypos; continue());
<INITIAL,C>{ws}		=> (continue()); 

<INITIAL>"/*"		=> (YYBEGIN C; continue());
<C>"*/"	 	=> (YYBEGIN INITIAL; continue());
<C>.		=> (continue());


<INITIAL>"/-"		=> (charlist := [""]; stringstart := yypos; YYBEGIN PC; continue());
<PC>{newline}	 	=> (YYBEGIN INITIAL; 
                            SourceMap.newline sourceMap yypos;
                            Tokens.PCOMMENT(makeString charlist, !stringstart, yypos));
<PC>.		        => (addString(charlist,yytext); continue());

<INITIAL>"//"   => (YYBEGIN CC; continue());
<CC>{newline}   => (YYBEGIN INITIAL; 
                    SourceMap.newline sourceMap yypos; continue());  
<CC>.           => (continue());

<INITIAL>\"		=> (charlist := [""]; stringstart := yypos; YYBEGIN S; continue());
<S>\"{ws}\"   =>   (continue());
<S>\"	        => (YYBEGIN INITIAL;Tokens.STRING(makeString charlist,!stringstart,yypos+1));
<S>\n		=> ((#err errWarn) (!stringstart,yypos,"unclosed string");
		    SourceMap.newline sourceMap yypos;
		    YYBEGIN INITIAL; Tokens.STRING(makeString charlist,!stringstart,yypos));
<S>[^"\\\n]*	=> (addString(charlist,yytext); continue());
<S>\\\n	       	=> (SourceMap.newline sourceMap yypos; continue());
<S>\\0		 => (addString(charlist,chr 0);continue());
<S>\\{octdigit}{3} => (addString(charlist, chr(mkOctChar(substring(yytext, 1, size(yytext)-1), yypos, yypos+size(yytext), errWarn))); continue());
<S>\\x{hexdigit}+ => (addString(charlist, chr(mkHexChar(substring(yytext, 2, size(yytext)-2), yypos, yypos+size(yytext), errWarn))); continue());
<S>\\\^[@-_]	=> (addString(charlist,chr(ordof(yytext,2)-ord("@"))); continue());
<S>\\.     	=> (addString(charlist, chr(special_char(yytext, yypos, yypos+size(yytext), errWarn))); continue());

<INITIAL>":"		=> (Tokens.COLON(yypos,yypos+1));
<INITIAL>";"		=> (Tokens.SEMICOLON(yypos,yypos+1));
<INITIAL>"("		=> (Tokens.LPAREN(yypos,yypos+1));
<INITIAL>")"		=> (Tokens.RPAREN(yypos,yypos+1));
<INITIAL>"["		=> (Tokens.LBRACE(yypos,yypos+1));
<INITIAL>"]"		=> (Tokens.RBRACE(yypos,yypos+1));
<INITIAL>"{"		=> (Tokens.LCURLY(yypos,yypos+1));
<INITIAL>"}"		=> (Tokens.RCURLY(yypos,yypos+1));
<INITIAL>"."		=> (Tokens.DOT(yypos,yypos+1));
<INITIAL>"..."	        => (Tokens.ELIPSIS(yypos,yypos+3));
<INITIAL>","		=> (Tokens.COMMA(yypos,yypos+1));
<INITIAL>"*"		=> (Tokens.TIMES(yypos,yypos+1));
<INITIAL>"!"		=> (Tokens.BANG(yypos,yypos+1));
<INITIAL>"^"		=> (Tokens.HAT(yypos,yypos+1));
<INITIAL>"+"		=> (Tokens.PLUS(yypos,yypos+1));
<INITIAL>"-"		=> (Tokens.MINUS(yypos,yypos+1));
<INITIAL>"++"		=> (Tokens.INC(yypos,yypos+2));
<INITIAL>"--"		=> (Tokens.DEC(yypos,yypos+2));
<INITIAL>"->"		=> (Tokens.ARROW(yypos,yypos+1));
<INITIAL>"/"		=> (Tokens.DIVIDE(yypos,yypos+1));
<INITIAL>"~"	        => (Tokens.TILDE(yypos,yypos+1));
<INITIAL>"?"		=> (Tokens.QUESTION(yypos,yypos+1));
<INITIAL>"|"		=> (Tokens.BAR(yypos,yypos+1));
<INITIAL>"&"		=> (Tokens.AMP(yypos,yypos+1));
<INITIAL>"%"		=> (Tokens.PERCENT(yypos,yypos+1));
<INITIAL>"<="		=> (Tokens.LTE(yypos,yypos+2));
<INITIAL>">="		=> (Tokens.GTE(yypos,yypos+2));
<INITIAL>"=="		=> (Tokens.EQ(yypos,yypos+2));
<INITIAL>"="	        => (Tokens.EQUALS(yypos,yypos+1));
<INITIAL>"+="		=> (Tokens.PLUSEQUALS(yypos,yypos+2));
<INITIAL>"-="		=> (Tokens.MINUSEQUALS(yypos,yypos+2));
<INITIAL>"^="		=> (Tokens.XOREQUALS(yypos,yypos+2));
<INITIAL>"%="		=> (Tokens.MODEQUALS(yypos,yypos+2));
<INITIAL>"*="		=> (Tokens.TIMESEQUALS(yypos,yypos+2));
<INITIAL>"/="		=> (Tokens.DIVEQUALS(yypos,yypos+2));
<INITIAL>"|="		=> (Tokens.OREQUALS(yypos,yypos+2));
<INITIAL>"&="		=> (Tokens.ANDEQUALS(yypos,yypos+2));
<INITIAL>"<<="	        => (Tokens.LSHIFTEQUALS(yypos,yypos+3));
<INITIAL>">>="	        => (Tokens.RSHIFTEQUALS(yypos,yypos+3));
<INITIAL>"<"		=> (Tokens.LT(yypos,yypos+1));
<INITIAL>">"		=> (Tokens.GT(yypos,yypos+1));
<INITIAL>"!="		=> (Tokens.NEQ(yypos,yypos+2));
<INITIAL>"||"		=> (Tokens.OR(yypos,yypos+2));
<INITIAL>"&&"		=> (Tokens.AND(yypos,yypos+2));
<INITIAL>"<<"		=> (Tokens.LSHIFT(yypos,yypos+2));
<INITIAL>">>"		=> (Tokens.RSHIFT(yypos,yypos+2));

<INITIAL>"(:"		=> (Tokens.LCOLON(yypos,yypos+2)  (* PADS *));  
<INITIAL>":)"		=> (Tokens.RCOLON(yypos,yypos+2));
<INITIAL>".."		=> (Tokens.DOTDOT(yypos,yypos+2));
<INITIAL>"::"		=> (Tokens.COLONCOLON(yypos,yypos+2));
<INITIAL>"=>"		=> (Tokens.EQGT(yypos,yypos+2));


<INITIAL>{octnum}	=> (Tokens.DECNUM(mkOctInt(yytext,yypos,yypos+size(yytext),errWarn),yypos, yypos+size(yytext)));
<INITIAL>{hexnum}	=> (Tokens.DECNUM(mkHexInt(yytext,yypos,yypos+size(yytext),errWarn),yypos, yypos+size(yytext)));
<INITIAL>{decnum}	=> (Tokens.DECNUM(mkInt (yytext,yypos,yypos+size(yytext),errWarn), yypos,yypos+size(yytext)));
<INITIAL>{realnum}      =>
(Tokens.REALNUM(mkRealNum(yytext,yypos,yypos+size(yytext),errWarn), yypos, yypos
+ size(yytext)));

<INITIAL>"'\\"{octdigit}{1,3}"'"	=> (let val s = substring(yytext, 2, size(yytext)-3)
				     in Tokens.CCONST(IntInf.fromInt (mkOctChar(s,yypos,yypos+size(yytext),errWarn)),
						      yypos,
					      yypos+size(yytext))
	                             end);

<INITIAL>"'\\x"{hexdigit}+"'"	=>  (let val s = substring(yytext, 3, size(yytext)-4)
				     in Tokens.CCONST(IntInf.fromInt (mkHexChar(s,yypos,yypos+size(yytext),errWarn)),
						      yypos,
						      yypos+size(yytext))
	                             end);


<INITIAL>{simplecharconst}	=> (let val cval = ordof(yytext,1)
	                            in Tokens.CCONST(IntInf.fromInt cval,yypos,yypos+size(yytext))
                                    end);
<INITIAL>{escapecharconst} => (Tokens.CCONST(IntInf.fromInt(special_char(substring(yytext,1,size(yytext)-2),yypos,yypos+size(yytext),errWarn)), yypos, yypos+size(yytext)));
<INITIAL>{id}        	=> (TTokTable.checkToken(yytext,yypos));
<INITIAL>.        	=> (continue());

