{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell { buildInputs = with pkgs; [ gcc clang-tools python3 ]; }

