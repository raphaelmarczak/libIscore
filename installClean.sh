#!/bin/sh

cp /Users/nicolashincker/Documents/workspace/i-score/dev/i-score/i-score.app .
cd macdeployqt/macdeployqt/
./macdeployqt ../../i-score.app -iscore
