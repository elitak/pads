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
structure TyProps =

struct
   structure P = ParseTreeUtil
   structure PT = ParseTree
   structure PX = ParseTreeExt

   datatype diskSize =  Size of IntInf.int * IntInf.int  (* number of bytes, number of EOR markers *)
                      | Param of string list * string option * PT.expression * PT.expression
                      | Variable
   (* XXX: Change pexp to pcexp to be consistent with cnv-ext.sml? *)
   type pexp = PT.expression
   type argList = string list * pexp list

   (* label name, label type, supplied arguments, is omitted, comment *)
   type labelInfo = string * string * argList * bool * string option

   datatype compoundSize =  Base  of diskSize 
                          | Try   of diskSize * string * argList
                          | Trans of diskSize * string * argList
                          | Typedef of diskSize * string * argList
                          | Recursive of diskSize * string * argList
                          | Struct of (labelInfo option * diskSize) list 
                          | Union of (labelInfo option * diskSize) list 
                          | Array of {baseTy : string, args : argList,
				      elem : diskSize, sep : diskSize, term : diskSize, length : diskSize}
                          | Enum of diskSize

   type tyApp = {tyCon: string, args : pexp list}
   datatype pType = Pads of tyApp | C of Ast.ctype

   datatype literalTy = DChar of pexp | DString of pexp | DRegExp of pexp | DNoSep


   type baseInfoTy = unit
   type tryInfoTy = {ds : diskSize,
		     paramNames : string list,
		     baseName : string,
		     baseArgs : pexp list}

   type typedefInfoTy = tryInfoTy


   type transInfoTy = {ds: diskSize,
		       paramNames : string list,
		       srcName : string,
		       srcArgs : pexp list,
		       dstName : string,
		       dstArgs : pexp list,
		       sToD    : pexp,
		       sToDArgs: pexp list,
		       dToS    : pexp,
		       dToSArgs: pexp list}


   type recursiveInfoTy = {base : tyApp} (* Describe the underlying type *)

   datatype fieldInfoTy =  Full of {ty:tyApp, name:string, pred : pexp PX.PPostCond list, comment: string option,
				    isOpt : bool, optPred : pexp PX.OptPredicate option,
				    isArray : bool, size: (pexp PX.PSize) option, arrayPred : (pexp PX.PConstraint) list}
                         | Literal of literalTy
                         | Compute of {ty:pType, name:string, def:pexp, pred: pexp PX.PPostCond list, comment:string option}
   type structInfoTy = {fields:fieldInfoTy list, pred: pexp PX.PPostCond list}
   val defStructInfo:structInfoTy = {fields = [], pred = []}

   type unionInfoTy = {fromOpt: bool, descriminator : pexp option, cases : pexp option list, 
		       fields:fieldInfoTy list, pred: pexp PX.PPostCond list}
   val defUnionInfo = {fromOpt=false, descriminator=NONE, cases = [], fields=[], pred=[]}

   type delimTy = {sep:literalTy option, term:literalTy option, preds:pexp PX.PConstraint list}
   type sizeTy = {min: pexp option, max: pexp option, minConst : (IntInf.int*bool) option, maxConst : (IntInf.int *bool) option}
   type arrayPostConds = pexp PX.PArrayPostCond list
   type arrayInfoTy = {baseTy:tyApp, delims : delimTy, size:sizeTy, post:arrayPostConds}
   val defArrayInfo:arrayInfoTy = {baseTy={tyCon="Error",args=[]}, 
				   delims={sep=NONE,term=NONE, preds=[]},
				   size={min=NONE,max=NONE,minConst = NONE, maxConst =NONE},
				   post=[]}

   type enumInfoTy = unit
   datatype tyInfo = BaseInfo of baseInfoTy
                   | TransInfo of transInfoTy 
                   | TryInfo of tryInfoTy 
                   | TypedefInfo of typedefInfoTy 
                   | RecursiveInfo of recursiveInfoTy 
                   | StructInfo of structInfoTy
                   | UnionInfo of unionInfoTy
                   | ArrayInfo of arrayInfoTy
                   | EnumInfo of enumInfoTy
           

   datatype memChar = Static | Dynamic

   fun mkSize (n1,n2) = Size(IntInf.fromInt n1, IntInf.fromInt n2)

   fun printStrList [] = ""
     | printStrList (x::xs) = (x^", "^( printStrList xs))

   fun printSize Variable = print "Variable\n"
     | printSize (Param(params, _, exp1, exp2)) = 
          print ("Parameterized:\n Vars: "^(printStrList params)^
		 "\nNumber of bytes expression: "^(P.expToString exp1)^
		 "\nNumber of records expresion: "^(P.expToString exp2)^".\n")
     | printSize (Size(n1,n2)) = print ("Fixed size: bytes = "^(IntInf.toString n1)^" records = "^(IntInf.toString n2)^".\n")

   fun add (Variable, _ ) = Variable 
     | add (_, Variable ) = Variable 
     | add (Size(x1,y1), Size(x2,y2)) = Size(IntInf.+(x1, x2), IntInf.+(y1, y2))
     | add (Size(x1,y1), Param(ps, s, ebytes, erecs)) = Param(ps, s, P.plusX(PT.IntConst x1, ebytes),
							             P.plusX(PT.IntConst y1, erecs))
     | add (Param(ps, s, ebytes, erecs), Size(x2,y2)) = Param(ps, s, P.plusX(ebytes, PT.IntConst x2),
							             P.plusX(erecs, PT.IntConst y2))
     | add (Param(ps1,s1,ebytes1, erecs1), Param(ps2,s2,ebytes2,erecs2)) = 
             Param(ps1, NONE, P.plusX(ebytes1, ebytes2), P.plusX(erecs1, erecs2))


   val dynamicValue = PT.Call(PT.Id "strlen", [PT.String "Non-static-length"])
   fun merge (e1,e2) =  P.condX(P.eqX(e1, e2), e2, dynamicValue)

   fun overlay (Variable, _ ) = Variable 
     | overlay (_, Variable ) = Variable 
     | overlay (r as Size(x1,y1), Size(x2,y2)) = 
       if x1 = x2 andalso y1 = y2 then  r else Variable
     | overlay (Size(x1,y1), Param(ps, s, ebytes, erecs)) = 
	   Param(ps, s, merge(PT.IntConst x1, ebytes), merge(PT.IntConst y1, erecs))
     | overlay (Param(ps, s, ebytes, erecs), Size(x2,y2)) = 
	   Param(ps, s, merge(ebytes, PT.IntConst x2), merge(erecs, PT.IntConst y2))
     | overlay (Param(ps1,s1,ebytes1, erecs1), Param(ps2,s2,ebytes2,erecs2)) = 
             Param(ps1, NONE, merge(ebytes1, ebytes2), merge(erecs1, erecs2))

   (* scale first argument by first first component of second;
      using byte size to represent array repetition. *)
   fun scale (Variable, _ ) = Variable 
     | scale (_, Variable ) = Variable 
     | scale (Size(x1,y1), Size(rep,_)) = Size(IntInf.*(x1, rep), IntInf.*(y1, rep))
     | scale (Size(x1,y1), Param(ps, s, rep, _)) = Param(ps, s, P.timesX(PT.IntConst x1, rep),
							        P.timesX(PT.IntConst y1, rep))
     | scale (Param(ps, s, rep, _), Size(x2,y2)) = Param(ps, s, P.timesX(rep, PT.IntConst x2),
							          P.timesX(rep, PT.IntConst y2))
     | scale (Param(ps1,s1,ebytes1, erecs1), Param(ps2,s2,rep,_)) = 
             Param(ps1, NONE, P.timesX(ebytes1, rep), P.plusX(erecs1, rep))



   fun mergeDiskSize f (x,y) = case (x,y) 
    of (Variable, _ ) => Variable
     | (_, Variable ) => Variable
     | (Param a, _) => Param a
     | (_, Param b) => Param b
     | (Size n, Size m) => f(n,m)

   fun mergeMemChar (Dynamic, _) = Dynamic
     | mergeMemChar (_, Dynamic) = Dynamic
     | mergeMemChar (Static, Static) = Static


end
