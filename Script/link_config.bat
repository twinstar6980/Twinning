@echo off
setlocal enabledelayedexpansion

set src=C:\Users\TwinKleS\Documents\Code\TwinKleS\ToolKit\script
set dst=C:\Users\TwinKleS\Documents\Code\TwinKleS\ToolKit\script\.build

set list[1]=Language\Language.json
set list[2]=Entry\Entry.json
set list[3]=Entry\method\js.json
set list[4]=Entry\method\json.json
set list[5]=Entry\method\data.hash.json
set list[6]=Entry\method\data.encode.json
set list[7]=Entry\method\data.encrypt.json
set list[8]=Entry\method\data.compress.json
set list[9]=Entry\method\image.atlas.json
set list[10]=Entry\method\wwise.encoded_media.json
set list[11]=Entry\method\wwise.sound_bank.json
set list[12]=Entry\method\marmalade.dzip.json
set list[13]=Entry\method\popcap.zlib.json
set list[14]=Entry\method\popcap.reanim.json
set list[15]=Entry\method\popcap.rton.json
set list[16]=Entry\method\popcap.ptx.json
set list[17]=Entry\method\popcap.pam.json
set list[18]=Entry\method\popcap.pak.json
set list[19]=Entry\method\popcap.rsgp.json
set list[20]=Entry\method\popcap.rsb.json
set list[21]=Entry\method\misc.pvz2.lawn_string_text.json
set list[22]=Entry\method\expand.json

for /l %%i in (1, 1, 22) do (
    mklink %dst%\!list[%%i]! %src%\!list[%%i]!
)

pause