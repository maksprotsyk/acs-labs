import subprocess
import sys
import math
# Uncomment to build a plot of the measured time
# import matplotlib.pyplot as plt


RELATIVE_ERROR = 2 * 10 ** -12
ABSOLUTE_ERROR = 8 * 10 ** -6
PROGRAM_PATH = "./bin/calculator"
TEMP_DATA_FOLDER = "data/"
DEFAULT_BORDERS = [(-50.0, -50.0), (50.0, 50.0)]


def check_error(value, new_value, abs_error=ABSOLUTE_ERROR,
                rel_error=RELATIVE_ERROR):
    """
    Checks if relative error and absolute error
    are below the given level
    """
    diff = abs(value - new_value)
    return diff < abs_error and diff/new_value < rel_error


def build_configuration_file(
      borders, abs_error, rel_error,
      threads, config_file
      ):
    """
    Creates configuration file for c++
    integral calculation program
    """
    with open(config_file, "w") as f:
        f.write(f"{borders[0][0]} {borders[1][0]}"
                f" {borders[0][1]} {borders[1][1]}"
                f" {abs_error} {rel_error} {threads}")


def perform_run(config_file, output, n):
    """
    Performs n runs of the c++ integral
    calculation program and returns the value
    of the integral and minimal time
    """
    for i in range(n):
        res = subprocess.run([PROGRAM_PATH,
                              config_file, output])
        if res.returncode != 0:
            print("Error while running integral calculator")
            return

    value = None
    min_time = math.inf
    with open(output) as f:
        for line in f.readlines():
            line = line.split()
            if value is None:
                value = float(line[0])
            else:
                new_value = float(line[0])
                abs_error = abs(new_value - value)
                if not check_error(value, new_value):
                    return None, None

            min_time = min(float(line[3]), min_time)

    return min_time, value


def test_program(config_file, output, n=10, threads=9):
    """
    Tests program n times for amounts of threads from 1
    to the given value. (Compares result) Saves time results to
    the output file and returns the list of measured mininal
    times
    """
    measurements = []
    value = None
    f = open(output, "w")
    for i in range(1, threads+1):
        build_configuration_file(DEFAULT_BORDERS,
                                 ABSOLUTE_ERROR,
                                 RELATIVE_ERROR, i,
                                 config_file)
        min_time, new_value = perform_run(
            config_file,
            TEMP_DATA_FOLDER + f"output{i}.txt", n
            )
        if value is None:
            value = new_value
        if min_time is None or not check_error(value, new_value):
            print(f"Error in results while testing for {i} threads")
            return

        measurements.append(min_time)
        f.write(f"{i} threads: {min_time}\n")
    f.close()
    return measurements


def plot_measurements(measurements, output):
    """
    Plots measured time and saves the plot to output file
    """
    plt.plot(range(1, len(measurements) + 1), measurements)
    plt.xlabel("Threads")
    plt.ylabel("Measured time in seconds")
    plt.ylim([0, max(measurements) * 1.1])
    plt.savefig(output)
    plt.show()


def main():
    args = sys.argv
    if len(args) != 4:
        print("Usage python3 program_name temp_config_file output cycles_num")
        return
    try:
        n = int(args[3])
        measurements = [time/1000000 for time in test_program(*args[1:3], n)]
        for i in range(1, len(measurements) + 1):
            print(f"{i} threads: {measurements[i-1]}")
        # Uncomment to build a plot of the measured time
        # plot_measurements(measurements, "img/plot.jpg")
    except ValueError:
        print("Invalid args")


if __name__ == "__main__":
    main()
