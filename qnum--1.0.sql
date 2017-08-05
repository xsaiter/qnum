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

--create domain vlong_text as text check (value ~ '^[0-9]+$');
--create type vlong as (s vlong_text);

create or replace function vlong(s_ text)
returns vlong as
$$
begin
  return row(s_)::vlong;
end  
$$
language plpgsql;

create function q_num_vlong_add (text, text) returns text
as '$libdir/libqnum'
language C immutable strict;