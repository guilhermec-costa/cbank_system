#!/bin/bash
ls src/**/*.{c,h,html} | entr -r sh -c "make fmt && make && ./bin/bank"
