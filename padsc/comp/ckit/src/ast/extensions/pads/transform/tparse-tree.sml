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
structure TParseTree : TPARSETREE =
struct
  datatype sTy = TyName | TyDecl 
  datatype dTy = Str | IO
  datatype conTy = Pstruct | Punion | Palternate | Parray | Ptypedef | Penum

  datatype targ = ArgS of string | ArgC of conTy * string

  datatype body 
         = Bsimple of string 
         | Bstruct of {pre : string option, each: (string * string * string option * string) option, post : string option}
       
  datatype externalDecl 
         = Output of string
         | TransformTy of string * sTy * dTy
         | Transform of string * targ * body
         | MARKexternalDecl of (SourceMap.location * externalDecl)  

end (* structure TParseTree *)
