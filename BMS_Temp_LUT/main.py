import csv
import string

C_TEMPLATE_FILENAME = "c_template.txt"
H_TEMPLATE_FILENAME = "h_template.txt"
C_FILENAME = "FEB_Temp_LUT.c"
H_FILENAME = "FEB_Temp_LUT.h"
TEMP_MAP_FILENAME = "values.csv"

VOLT_RESOLUTION_POWER = 3
VOLT_RESOLUTION = 10 ** -VOLT_RESOLUTION_POWER
TEMP_RESOLUTION_POWER = 1

# Temperature map, sorted by voltage
def read_values() -> list[tuple[float, float]]:
    values = []
    with open(TEMP_MAP_FILENAME, "r") as f:
        csv_reader = csv.reader(f)
        next(csv_reader)    # Skip header
        for row in csv_reader:
            temp, volt = float(row[0]), float(row[1])
            values.append((volt, temp))
    values = sorted(values, key=lambda x: x[0])
    min_volt_mV = int(values[0][0] * 10 ** VOLT_RESOLUTION_POWER)
    max_volt_mV = int(values[-1][0] * 10 ** VOLT_RESOLUTION_POWER)
    return values, min_volt_mV, max_volt_mV

def linear_interpolate(x: float, x1: float, y1: float, x2: float, y2: float) -> float:
    return y1 + (x - x1) * (y2 - y1) / (x2 - x1)

def interpolate_values(values: list) -> list:
    temp_list = []
    for i in range(len(values) - 1):
        x1, y1 = values[i]
        x2, y2 = values[i + 1]
        curr_volt = x1
        while curr_volt < x2:
            curr_temp = linear_interpolate(curr_volt, x1, y1, x2, y2)
            curr_temp = round(curr_temp, TEMP_RESOLUTION_POWER)
            curr_temp = int(curr_temp * 10 ** TEMP_RESOLUTION_POWER)
            temp_list.append(curr_temp)
            curr_volt = round(curr_volt + VOLT_RESOLUTION, VOLT_RESOLUTION_POWER)
    curr_temp = round(values[-1][1], TEMP_RESOLUTION_POWER)
    curr_temp = int(curr_temp * 10 ** TEMP_RESOLUTION_POWER)
    temp_list.append(curr_temp)
    return temp_list

# Reference function to index temperature list
def get_temp(volt_mV: int, temp_list: list, min_volt_mV: int, max_volt_mV: int) -> float:
    if volt_mV < min_volt_mV or volt_mV > max_volt_mV:
        return -42
    index = (volt_mV - min_volt_mV)
    return temp_list[index]

def write_c_file(temp_list: list) -> None:
    file_data = {
        "lut_values": str(temp_list)[1:-1],
        "lut_length": len(temp_list),
    }
    with open(C_TEMPLATE_FILENAME, "r") as f:
        template = string.Template(f.read())
    with open(C_FILENAME, "w") as f:
        f.write(template.substitute(file_data))

def write_h_file(min_volt_mV: int, max_volt_mV: int) -> None:
    file_data = {
        "min_volt": min_volt_mV,
        "max_volt": max_volt_mV,
    }
    with open(H_TEMPLATE_FILENAME, "r") as f:
        template = string.Template(f.read())
    with open(H_FILENAME, "w") as f:
        f.write(template.substitute(file_data))
        
def main() -> None:
    values, min_volt_mV, max_volt_mV = read_values()
    temp_list = interpolate_values(values)
    write_c_file(temp_list)
    write_h_file(min_volt_mV, max_volt_mV)

if __name__ == "__main__":
    main()
