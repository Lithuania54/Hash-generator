import random
import string
import os

def generate_random_string(length):
    return ''.join(random.choices(string.ascii_lowercase + string.digits, k=length))

def generate_string_pair(length):
    str1 = generate_random_string(length)
    index_to_change = random.randint(0, length - 1)
    str2 = str1[:index_to_change] + random.choice(string.ascii_lowercase + string.digits) + str1[index_to_change + 1:]
    return str1, str2

def save_string_pairs(filename, num_pairs, max_length):
    os.makedirs(os.path.dirname(filename), exist_ok=True)
    with open(filename, 'w') as file:
        for _ in range(num_pairs):
            length = random.randint(1, max_length)
            str1, str2 = generate_string_pair(length)
            file.write(f'{str1},{str2}\n')

save_string_pairs('test_files/different_by_one_symbol.txt', 100000, 1000)
