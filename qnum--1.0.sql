-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "create extension qnum" to load this file. \quit

create function q_num_is_prime (int8) returns boolean
as '$libdir/libqnum'
language C immutable strict;


create function q_num_gcd (int8, int8) returns int8
as '$libdir/libqnum'
language C immutable strict;


create function q_num_is_relatively_prime (int8, int8) returns boolean
as '$libdir/libqnum'
language C immutable strict;


-- create
create domain vlong_text as text check (value ~ '^[0-9]+$');


-- add
create function q_num_vlong_add (vlong_text, vlong_text) returns vlong_text
as '$libdir/libqnum'
language C immutable strict;

create operator `+ (
  leftarg = vlong_text,
  rightarg = vlong_text,
  procedure = q_num_vlong_add
);


-- eq
create function q_num_vlong_eq (vlong_text, vlong_text) returns boolean
as '$libdir/libqnum'
language C immutable strict;

create operator `== (
  leftarg = vlong_text,
  rightarg = vlong_text,
  procedure = q_num_vlong_eq
);


-- gt
create function q_num_vlong_gt (vlong_text, vlong_text) returns boolean
as '$libdir/libqnum'
language C immutable strict;

create operator `> (
  leftarg = vlong_text,
  rightarg = vlong_text,
  procedure = q_num_vlong_gt
);
