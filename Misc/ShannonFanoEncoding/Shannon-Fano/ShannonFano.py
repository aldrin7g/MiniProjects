import re
import numpy as np
from PIL import Image
import collections
import os
import math

# Function to create the Results folder if not already present
def create_results_folder():
    if not os.path.exists('Results'):
        os.makedirs('Results')
        
create_results_folder()

print("\nShannon Image Compression Program")
print("=================================================================")

h = int(input("\nInput Colour(1)/Grayscale(2): "))

if h == 1:
    file = input("\nEnter the filename: ")
    my_string = np.asarray(Image.open(file), np.uint8)
    sudhi = my_string
    shape = my_string.shape
    print("\n\nEntered image data is:")
    message = str(my_string.tolist())
elif h == 2:
    array = np.arange(0, 737280, 1, np.uint8)
    my_string = np.reshape(array, (1024, 720))
    print("\n\nEntered image data is:")
    message = str(my_string.tolist())
    sudhi = my_string
else:
    print("\n\nYou entered an invalid input")
    exit()

# Save raw image data
with open('Results/rawdata.txt', 'w') as file:
    file.write(message)

c = {}  # Define the dictionary `c` here

def create_list(message):
    list = dict(collections.Counter(message))
    total_count = sum(list.values())
    for key, value in list.items():
        probability = round(value / total_count, 4)
        print(key, ' :  Count: ', value, ' Probability: ', probability)
    list_sorted = sorted(list.items(), key=lambda k_v: (k_v[1], k_v[0]), reverse=True)
    final_list = []
    for key, value in list_sorted:
        final_list.append([key, value, ''])
    print("\n")
    print("Shannon Fano Process:")
    return final_list

def divide_list(list):
    if len(list) == 2:
        return [list[0]], [list[1]]
    else:
        n = sum(i[1] for i in list)
        x = 0
        distance = abs(2 * x - n)
        j = 0
        for i in range(len(list)):
            x += list[i][1]
            if distance < abs(2 * x - n):
                j = i
        return list[0:j+1], list[j+1:]

def label_list(list):
    list1, list2 = divide_list(list)
    for i in list1:
        i[2] += '0'
        c[i[0]] = i[2]
    for i in list2:
        i[2] += '1'
        c[i[0]] = i[2]
    print(f"{list1} :: {list2}\n")
    if len(list1) == 1 and len(list2) == 1:
        return
    label_list(list2)
    return c

code = label_list(create_list(message))
print("\nShannon's Encoded Code:")

letter_binary = []
for key, value in code.items():
    print(key, ' : ', value)
    letter_binary.append([key, value])

# Save symbol statistics and store probability and code length in a list
symbol_stats = []
with open('Results/symbolstats.txt', 'w') as file:
    for key, value in code.items():
        probability = message.count(key) / len(message)
        file.write(f"Symbol: {key}, Probability: {probability:.6f}, Code: {value}\n")
        symbol_stats.append({'Symbol': key, 'Probability': probability, 'Code Length': len(value)})

# Compress the message
with open('Results/compressed.txt', 'w') as output:
    for a in message:
        for key, value in code.items():
            if key in a:
                output.write(value)

# Decode the message
with open('Results/compressed.txt', 'r') as output:
    bitstring = output.read()

uncompressed_string = ""
code = ""
for digit in bitstring:
    code += digit
    pos = 0
    for letter in letter_binary:
        if code == letter[1]:
            uncompressed_string += letter_binary[pos][0]
            code = ""
        pos += 1

# Save uncompressed data
with open('Results/uncompressed.txt', 'w') as file:
    file.write(uncompressed_string)

# Save uncompressed image
if h == 1:
    temp = re.findall(r'\d+', uncompressed_string)
    res = list(map(int, temp))
    res = np.array(res)
    res = res.astype(np.uint8)
    res = np.reshape(res, shape)
    print("\n\nObserve the shapes and input and output arrays are matching or not")
    print("Input image dimensions:", shape)
    print("Output image dimensions:", res.shape)
    data = Image.fromarray(res)
    data.save('Results/uncompressed.png')
    if np.array_equal(sudhi, res):
        print("Success\n")
elif h == 2:
    temp = re.findall(r'\d+', uncompressed_string)
    res = list(map(int, temp))
    res = np.array(res)
    res = res.astype(np.uint8)
    res = np.reshape(res, (1024, 720))
    data = Image.fromarray(res)
    data.save('Results/uncompressed.png')
    print("\nSuccess\n")

# Print symbol statistics
print("Symbol Statistics:")
entropy = 0
lavg = 0
efficiency = 0

for stat in symbol_stats:
    
    p = stat['Probability']
    l = stat['Code Length']
    entropy = entropy + (p * math.log2(1/p))
    lavg = lavg + (p * l)

efficiency = entropy/lavg

print(f"Entropy: {entropy:.4f}\nAverage CodeWordLength: {lavg:.4f}\nEfficiency: {efficiency*100:.2f}%\n")
