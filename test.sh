rm -rf covConfig build
mkdir covConfig
cov-configure --config covConfig/coverity.xml --compiler cc --comptype gcc --template
cmake -B build -S test;
cd build/;
cov-build --config ../covConfig/coverity.xml --emit-complementary-info --dir cov-out make coverity_analysis
cd cov-out/
cov-analyze --dir . --coding-standard-config ../../tools/coverity/misra.config --tu-pattern "file('.*/source/.*')";
cov-format-errors --dir . --file "source" --exclude-files '(/build/|/test/)' --html-output html-out;
cov-format-errors --dir . --file "source" --exclude-files '(/build/|/test/)' --json-output-v2 defects.json;
echo -e "\n-------------------------Non-Suppresed Deviations, if any, Listed Below-------------------------\n";
jq '.issues[] | .events[] | .eventTag ' defects.json | sort | uniq -c | sort -nr;
echo -e "\n-------------------------Non-Suppresed Deviations, if any, Listed Above-------------------------\n";
cd ../../;