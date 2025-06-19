#!/bin/bash
ls src/**/*.c | entr -r sh -c "make && ./bin/bank"