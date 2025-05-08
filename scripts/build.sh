#rm -rf output
python build.py $1 $2 $3 && ninja -f SoundBuild.ninja && ninja -f GLTFBuild.ninja && ninja -f ShadersBuild$2.ninja && ninja -f Build$2$3.ninja || exit 1
if [ $2 == "Web" ]; then
    cd output/$4
    start chrome http://localhost:9222
    python -m http.server 9222
fi
exit 0
