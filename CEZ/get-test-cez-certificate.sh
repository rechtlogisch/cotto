#!/bin/bash

curl -sS -O https://download.elster.de/download/schnittstellen/Test_Zertifikate.zip && \
unzip -j Test_Zertifikate.zip eric-zertifikate-bescheidabholung/PSE/eric_private.p12 eric-zertifikate-bescheidabholung/PSE/eric_public.cer && \
rm Test_Zertifikate.zip
