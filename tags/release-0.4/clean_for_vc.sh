#!/bin/sh

rm -f ARMware.ncb
rm -f ARMware.suo

rm -rf `find . -name "Debug" -or -name "Release" -or -name "debug" -or -name "release"`
rm -rf `find . -name "*.user"`
