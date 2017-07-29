-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "create extension qnum" to load this file. \quit

create function q_num_is_prime (int8) returns boolean
as '$libdir/libqnum'
language C immutable strict;