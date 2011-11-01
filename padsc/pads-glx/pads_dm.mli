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
*                Mary Fernandez <mff@research.att.com>                 *
*                                                                      *
***********************************************************************)
(* 
   Module: Pads_dm
   Description:
     This module implements the PADS datamodel. 
*)

open Dm
open Pads_c


(* PADS Items *)

(* PADS Nodes *)

class virtual padsNode :
  Nodeid.docid ->  
  nodeRep -> 
  nodeRep option -> 
  object
    inherit node

    method implementation : unit -> string
    method get_implid : Nodeid.implemid
    method get_nodeid_context: Nodeid_context.nodeid_context

    method get_nodeRep   : nodeRep

  (* Axes *)

    method parent        : (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option  -> Dm.node option

  (* Node identity *)

    method nodeid        : unit  -> Nodeid.nodeid
    method docorder      : unit  -> Nodeid.docorder
    method update_parent : Dm.node -> unit
    method reset_parent : unit -> unit

  (* Updates *)
    method delete     	 : Dm.node -> unit
    method detach     	 : Dm.node -> unit
    method insert     	 : Dm.node Cursor.cursor -> Dm.node option -> unit
    method replace    	 : Dm.node Cursor.cursor -> Dm.node -> unit
    method replace_value : Dm.text -> unit
    method rename        : Datatypes.xs_QName -> unit

  end


(* PADS Document Nodes *)

class padsDocumentNode :
    Dm_atomic.atomicAnyURI option ref ->  (* optional base URI *)
    Nodeid.docid ->  
    string ->                  (* required .p file name *)
    nodeRep -> 
  object
    inherit document
    inherit padsNode

    method children 	: (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option -> Dm.node Cursor.cursor
    method document_uri : unit -> Dm_atomic.atomicString option

    method descendant_or_self   : (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option -> Dm.node Cursor.cursor
    method descendant           : (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option -> Dm.node Cursor.cursor
    method ancestor_or_self     : (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option -> Dm.node Cursor.cursor
    method ancestor             : (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option -> Dm.node Cursor.cursor

  end


(* PADS Element Nodes *)

class padsElementNode :
    Nodeid.docid ->  
    nodeRep ->
    Namespace_names.rqname ->
    nodeRep option ->
    Namespace_context.nsenv ->
  object
    inherit element
    inherit padsNode

    method children    : (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option -> Dm.node Cursor.cursor
    method node_name   : unit -> Dm_atomic.atomicQName option

    method node_type   : unit -> Namespace_symbols.rtype_symbol 

    method attributes  : (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option -> Dm.attribute Cursor.cursor
    method namespace_environment : unit -> Namespace_context.nsenv
    method elemName    : unit -> Namespace_symbols.relem_symbol
    method typed_value : unit -> Dm_atomic.atomicValue Cursor.cursor

    method export_typed_value : 
	unit -> Dm_atomic.atomicValue list

    method descendant_or_self   : (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option -> Dm.node Cursor.cursor
    method descendant           : (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option -> Dm.node Cursor.cursor
    method ancestor_or_self     : (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option -> Dm.node Cursor.cursor
    method ancestor             : (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option -> Dm.node Cursor.cursor

    method has_element_content : unit -> bool

    method nilled      : unit -> Dm_types.nilled
  end


(* PADS Text Nodes *)
class padsTextNode :
    Nodeid.docid ->  
    nodeRep ->
    nodeRep option ->
  object
    inherit text
    inherit padsNode

    method string_value : unit -> Datatypes.xs_string
    method typed_value : unit -> Dm_atomic.atomicValue Cursor.cursor

    method export_typed_value : 
	unit -> Dm_atomic.atomicValue list

    method descendant_or_self   : (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option -> Dm.node Cursor.cursor
    method descendant           : (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option -> Dm.node Cursor.cursor
    method ancestor_or_self     : (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option -> Dm.node Cursor.cursor
    method ancestor             : (Xquery_type_core_ast.cxschema option * Xquery_algebra_ast.anode_test) option -> Dm.node Cursor.cursor
(* End *)
  end

(* This is the "callback" function that is called from the PADS mainline *)
val pads_document : Processing_context.processing_context -> string -> string -> nodeRep -> Physical_value.item
val walk_pads_document : Physical_value.item -> unit
