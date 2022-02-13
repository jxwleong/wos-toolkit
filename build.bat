@echo off
set plog_include_path=%~dp0lib\plog\include

g++ foo.cpp -o foo.exe -I%plog_include_path%
foo.exe