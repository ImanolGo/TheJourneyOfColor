
export DISPLAY=:0
cd /home/morilondon/TheJourneyOfColor/OpenFrameworks/TheJourneyOfColorApp/bin/
ret=1
while [ $ret -ne 0 ]; do
    ./TheJourneyOfColorApp
    ret=$?
done
