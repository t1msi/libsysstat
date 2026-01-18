#!/usr/bin/env bash

# SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

sudo systemctl daemon-reload
sudo systemctl enable stats.service --now
