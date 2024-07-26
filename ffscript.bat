@echo off
set "input_directory=.\test"
set "output_directory=.\wav_files"

mkdir "%output_directory%" 2>nul

for %%i in ("%input_directory%\*.flac") do (
    ffmpeg -i "%%i" "%output_directory%\%%~ni.wav"
)