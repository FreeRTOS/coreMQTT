#!/usr/bin/bash
library="$(basename $PWD)"

	if [ -e "build/" ]; then
		rm -rf build
	fi

	cmake -S test -B build -G "Unix Makefiles" -DBUILD_CLONE_SUBMODULES=ON
	cd build
	cov-build --emit-complementary-info --dir cov-out make coverity_analysis;
	cd cov-out;
	cov-analyze --dir . --coding-standard-config ../../tools/coverity/misra.config 
	#cov-analyze --dir . --coding-standard-config ../../tools/coverity/misra.config --tu-pattern "file('.*/source/.*')";
	cov-format-errors --dir . --json-output-v2 all_defects.json;
	cov-format-errors --dir . --html-output html-out_all;
	echo -e "\n";
	echo -e "\n-------------------------${library}-------------------------\n";
	cov-format-errors --dir . --file "${library}/source" --exclude-files '(/dependency/|/3rdparty/|test/|/build/|/portable/)' --json-output-v2 ${library}_defects.json;
	#cov-format-errors --dir . --file "${library}/source" --exclude-files '(/dependency/|/3rdparty/|test/|/build/)' --json-output-v2 ${library}_defects.json;
	#cov-format-errors --dir . --file "${library}/source" --exclude-files '(/build/|/test/)' --json-output-v2 ${library}_defects.json;
	#cov-format-errors --dir . --file "${library}/source"  --json-output-v2 ${library}_defects.json;
	#cov-format-errors --dir . --json-output-v2 ${library}_defects.json;

	cov-format-errors --dir . --file "${library}/source" --exclude-files '(/dependency/|/3rdparty/|/test/|/build/|/portable/)' --html-output html-out_${library};
	#cov-format-errors --dir . --file "${library}/source" --exclude-files '(/dependency/|/3rdparty/|/test/|/build/)' --html-output html-out_${library};
	#cov-format-errors --dir . --file "${library}/source" --exclude-files '(/build/|/test/)' --html-output html-out_${library};
	#cov-format-errors --dir . --file "${library}/source"  --html-output html-out_${library};
	#cov-format-errors --dir . --html-output html-out_${library};

	echo -e "\n";
	if ! [ -e "../../lastRun.log" ]; then
		jq '.issues[] | .events[] | .eventTag '  ${library}_defects.json | sort | uniq -c | sort -nr > ../../lastRun.log;
	fi

	jq '.issues[] | .events[] | .eventTag '  ${library}_defects.json | sort | uniq -c | sort -nr;
	echo -e "\n";
	echo -e "\n-------------------------${library}-------------------------\n";
