g++ haha20.cpp ./lib_json/*.cpp -o submit/tophack-submission
# test
mkdir tmp
mkdir submit
cp submit/tophack-submission tmp/t1
cp submit/tophack-submission tmp/t2
cp submit/tophack-submission tmp/t3
java -jar ../Splendor.jar --player_binary_paths tmp/t1,tmp/t2,tmp/t3 --game_log_path tmp/log.txt --player_log_path tmp/stderr.txt

