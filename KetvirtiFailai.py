import random
import string

# Function to generate a random string of a given length
def generate_random_string(length):
    return ''.join(random.choices(string.ascii_lowercase + string.ascii_uppercase + string.digits, k=length))

# Function to generate string pairs of a specified length
def generate_string_pairs(num_pairs, length):
    pairs = []
    for _ in range(num_pairs):
        str1 = generate_random_string(length)
        str2 = generate_random_string(length)
        pairs.append((str1, str2))
    return pairs

# Generate 25,000 pairs for each length (10, 100, 500, 1000)
num_pairs = 25000
pairs_10 = generate_string_pairs(num_pairs, 10)
pairs_100 = generate_string_pairs(num_pairs, 100)
pairs_500 = generate_string_pairs(num_pairs, 500)
pairs_1000 = generate_string_pairs(num_pairs, 1000)

# Combine all pairs into a single list
all_pairs = pairs_10 + pairs_100 + pairs_500 + pairs_1000

# Output the pairs to a file for further use (optional)
with open('random_string_pairs.txt', 'w') as f:
    for pair in all_pairs:
        f.write(f"{pair[0]},{pair[1]}\n")

print("Random string pairs generated and saved to 'random_string_pairs.txt'")
