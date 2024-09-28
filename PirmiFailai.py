import random
import string

def generate_file(filename):
    # Generate random symbols for the file, >1000 symbols in length
    symbols = ''.join(random.choices(string.ascii_letters + string.digits, k=1001))

    # Create a modified version of the string with one character changed in the middle
    modified_symbols = list(symbols)
    mid_index = len(modified_symbols) // 2
    # Change the middle symbol to something else
    modified_symbols[mid_index] = random.choice(string.ascii_letters + string.digits)

    # Write the original and modified strings into two files
    with open(f'{filename}_original.txt', 'w') as f_original:
        f_original.write(symbols)

    with open(f'{filename}_modified.txt', 'w') as f_modified:
        f_modified.write(''.join(modified_symbols))

# Call the function to generate files
generate_file("test_files/symbol_difference")
