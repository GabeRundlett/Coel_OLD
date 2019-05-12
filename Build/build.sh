./Build/Premake/premake5 gmake
if [ "$#" -eq  "0" ]
then
	make
else
	make config="$1"
fi