target=hsmvs

all: ${target} clean

${target}: main.cpp basic.h weighting_saps.h
	g++ main.cpp -O3 -static -o ${target}

clean:
	rm -f *~

cleanup:
	rm -f ${target}
	rm -f *~
