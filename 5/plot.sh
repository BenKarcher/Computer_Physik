#!/bin/bash
#ffmpeg syntax for merging pngs and gnuplot based on Lukas Sabatchus' version
if gnuplot main.gnu; then
    ffmpeg -y -i plots/plot00/time%02d.png -vf scale="trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -profile:v high -pix_fmt yuv420p -g 25 -r 25 plots/h0.mp4
    ffmpeg -y -i plots/plot01/time%02d.png -vf scale="trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -profile:v high -pix_fmt yuv420p -g 25 -r 25 plots/h1.mp4
    ffmpeg -y -i plots/plot02/time%02d.png -vf scale="trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -profile:v high -pix_fmt yuv420p -g 25 -r 25 plots/h2.mp4
    ffmpeg -y -i plots/plot03/time%02d.png -vf scale="trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -profile:v high -pix_fmt yuv420p -g 25 -r 25 plots/h3.mp4
    ffmpeg -y -i plots/plotExact/time%02d.png -vf scale="trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -profile:v high -pix_fmt yuv420p -g 25 -r 25 plots/Exact.mp4
    for N in {1..9..1}; do
        ffmpeg -y -i plots/plotN1.$N/time%02d.png -vf scale="trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -profile:v high -pix_fmt yuv420p -g 25 -r 25 plots/N1.$N.mp4
    done
    ffmpeg -y -i plots/plotNOut/time%02d.png -vf scale="trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -profile:v high -pix_fmt yuv420p -g 25 -r 25 plots/Nalle.mp4
    ffmpeg -y -i plots/plotN3/time%02d.png -vf scale="trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -profile:v high -pix_fmt yuv420p -g 25 -r 25 plots/N3.mp4
    ffmpeg -y -i plots/plot03/time%02d.png -vf scale="trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -profile:v high -pix_fmt yuv420p -g 25 -r 25 plots/h3.mp4
fi
