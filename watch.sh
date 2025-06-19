#!/bin/bash
ls src/**/*.c | entr -r sh -c "make fmt && make && ./bin/bank"