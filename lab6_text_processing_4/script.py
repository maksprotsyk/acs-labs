import subprocess
import math
import sys

PROGRAM_PATH = "./bin/prog"
CONFIG_DEFAULT_PATH = "config.dat"
BY_ALPHABET_DEFAULT = "alph.txt"
BY_COUNT_DEFAULT = "count.txt"

def build_config(path, index_folder, by_alphabet, by_count, threads):
    with open(path, "w") as f:
        f.write(f"index_directory_path={index_folder}\n")
        f.write(f"by_alphabet_path={by_alphabet}\n")
        f.write(f"by_count_path={by_count}\n")
        f.write(f"tokens_num={threads}\n")


def run_many_times(n, by_alphabet, by_count, prog_path=PROGRAM_PATH, config_path=CONFIG_DEFAULT_PATH):
    by_alphabet_result = None
    by_count_result = None
    min_times = [math.inf, math.inf]
    for i in range(n):
        print(f"Run {i} started.")
        res = subprocess.run([prog_path,
                              config_path], capture_output=True)
        if res.returncode != 0:
                print("Error while running")
                return
        if by_alphabet_result is None:
            with open(by_alphabet) as f:
                by_alphabet_result = f.read()
        else:
            with open(by_alphabet) as f:
                new_by_alphabet_result = f.read()
            if new_by_alphabet_result != by_alphabet_result:
                print("Results aren't the same")
                return
        
        if by_count_result is None:
            with open(by_alphabet) as f:
                by_count_result = f.read()
        else:
            with open(by_alphabet) as f:
                new_by_count_result = f.read()
            if new_by_count_result != by_count_result:
                print("Results aren't the same")
                return
        new_times = list(map(int, res.stdout.decode().strip().split()))[-3:]
        for j in range(2):
            min_times[j] = min(min_times[j], new_times[j])
    return min_times, [by_alphabet_result, by_count_result]


def different_conf_run(n, threads, index_folder, by_alphabet=BY_ALPHABET_DEFAULT,
                       by_count=BY_COUNT_DEFAULT, config_path=CONFIG_DEFAULT_PATH):
    results = None
    times = []
    for i in range(1, threads+1):
        build_config(config_path, index_folder, by_alphabet, by_count, i)
        print(f"Using {i} thread{('s' if i > 1 else '')}:")
        res = run_many_times(n, by_alphabet, by_count, config_path=config_path)
        times.append(res[0])
        if results is None:
            results = res[1]
        else:
            if results[0] != res[1][0] or results[1] != res[1][1]:
                print("Runs with different amount of threads returned different dictionaries")
                return
    return times


def main():
    args = sys.argv
    if len(args) != 4:
        print("Usage python3 'progname' 'number of runs' 'indexing folder' 'threads num'")
        return -2
    
    try:
        n = int(args[1])
        index_folder = args[2]
        threads = int(args[3])
        print(different_conf_run(n, threads, index_folder))
    except TypeError:
        print("Bad arguments")
        return -1
    return 0

if __name__ == "__main__":
    main()


    

