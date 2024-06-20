import csv
import sys


def parse_csv(file_1_name, file_2_name, merged_file_name='merged_file.csv'):
    """
    This function reads a CSV file and prints the contents.

    Args:
        filename: The path to the CSV file.
    """

    # Open the 2 files for reading
    with open(file_1_name, 'r') as file_1:
        with open(file_2_name, 'r') as file_2:
            # Open the merged file for writing
            with open(merged_file_name, 'w') as merged_file:
                file_1_reader = csv.reader(file_1, delimiter=',')
                file_2_reader = csv.reader(file_2, delimiter=',')
                merged_file_writer = csv.writer(merged_file)

                for row_1, row_2 in zip(file_1_reader, file_2_reader):
                    merged_line = []
                    if len(row_1) != len(row_2):
                        exit(-1)

                    for i in range(len(row_1)):
                        cell = ""
                        if row_1[i] != '-1':
                            cell += row_1[i]
                        if row_2[i] != '-1':
                            cell += (" " + row_2[i])

                        merged_line.append(cell)

                    merged_file_writer.writerow(merged_line)


if __name__ == "__main__":
    if len(sys.argv) != 3 and len(sys.argv) != 4:
        print("Usage: ./script_name file_1_name.csv file_2_name.csv\
        merged_file_name.csv(optional)")
    # Replace 'your_file.csv' with the actual filename
    file_1_name = sys.argv[1]
    file_2_name = sys.argv[2]

    if len(sys.argv) == 3:
        parse_csv(file_1_name, file_2_name)
    elif len(sys.argv) == 4:
        parse_csv(file_1_name, file_2_name, sys.argv[3])
