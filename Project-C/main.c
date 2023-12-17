#include <stdio.h>  // Include the standard input/output library for reading and writing data.
#include <stdlib.h> // Include the standard library for memory allocation and process control.
#include <ctype.h>  // Include the character type library for character classification functions.
#include <string.h> // Include the string library for manipulating arrays of characters.

// Define a structure to represent a node in the Huffman tree.
struct HuffmanNode {
    char data; // The character associated with the node (if it is a leaf node).
    unsigned freq; // The frequency of the character.
    struct HuffmanNode* left, * right; // Pointers to the left and right child nodes in the tree.
};

// Define a structure for a priority queue that will be used to build the Huffman tree.
// This priority queue is implemented as a sorted array.
struct SortedArrayPriorityQueue {
    int size; // Current number of elements in the priority queue.
    int capacity; // Maximum capacity of the priority queue.
    struct HuffmanNode** array; // An array of pointers to Huffman nodes.
};

// Function to create a new Huffman tree node
struct HuffmanNode* createHuffmanNode(char data, unsigned freq) {
    // Allocate memory for a new HuffmanNode
    struct HuffmanNode* newNode = (struct HuffmanNode*)malloc(sizeof(struct HuffmanNode));

    // Check if memory allocation was successful
    if (newNode == NULL) {
        return NULL;
    }

    // Initialize the left and right child pointers to NULL
    newNode->left = NULL;
    newNode->right = NULL;

    // Set the character data and frequency
    newNode->data = data;
    newNode->freq = freq;

    // Return the pointer to the new node
    return newNode;
}


// Function to create a new sorted array priority queue
struct SortedArrayPriorityQueue* createSortedArrayPriorityQueue(int capacity) {
    // Dynamically allocate memory for a new priority queue
    struct SortedArrayPriorityQueue* queue =
        (struct SortedArrayPriorityQueue*)malloc(sizeof(struct SortedArrayPriorityQueue));

    // Check if memory allocation failed
    if (!queue) return NULL;

    // Initialize the size of the queue to 0, as it is empty initially
    queue->size = 0;
    // Set the capacity of the queue to the specified value
    queue->capacity = capacity;

    // Dynamically allocate memory for the array of HuffmanNode pointers
    // The size of the array is determined by the capacity of the queue
    queue->array =
        (struct HuffmanNode**)malloc(sizeof(struct HuffmanNode*) * capacity);

    // Check if memory allocation for the array failed
    if (!queue->array) {
        // If memory allocation failed, free the previously allocated memory for the queue
        free(queue);
        // Return NULL to indicate failure in creating the queue
        return NULL;
    }

    // Return the pointer to the newly created queue
    return queue;
}


// Function to insert a node into the sorted array priority queue
void insertSortedArrayPriorityQueue(struct SortedArrayPriorityQueue* queue, struct HuffmanNode* node) {
    // Check if the queue is already full
    if (queue->size == queue->capacity) {
        // If the queue is full, no more nodes can be added.
        // In a complete implementation, you might want to expand the queue or handle this differently.
        return;
    }

    // Find the correct position to insert the new node we start from the end of the current queue and move backwards.
    int i = queue->size - 1;
    // Keep moving backwards as long as we find nodes with a higher frequency than the new node.
    while (i >= 0 && queue->array[i]->freq > node->freq) {
        // Shift the node at current position to the next position to make space for the new node.
        queue->array[i + 1] = queue->array[i];
        i--; // Move to the previous node.
    }
    // Insert the new node into the correct position found.
    queue->array[i + 1] = node;
    // Increment the size of the queue as a new node has been added.
    queue->size++;
}



// Function to extract the node with the smallest frequency from the priority queue
struct HuffmanNode* extractMin(struct SortedArrayPriorityQueue* queue) {
    if (queue->size == 0) {
        return NULL; // Queue is empty
    }

    struct HuffmanNode* minNode = queue->array[0];

    // Shift all elements to the left
    for (int i = 1; i < queue->size; i++) {
        queue->array[i - 1] = queue->array[i];
    }
    queue->size--;

    return minNode;
}

// Function to build the Huffman tree and return the root node.
struct HuffmanNode* buildHuffmanTree(char data[], unsigned freq[], int size) {
    struct HuffmanNode* left, * right, * top;

    // Create a priority queue using a sorted array to store the nodes of the Huffman tree.
    struct SortedArrayPriorityQueue* queue = createSortedArrayPriorityQueue(size);
    // Insert all characters with their respective frequencies into the priority queue.
    for (int i = 0; i < size; ++i) {
        insertSortedArrayPriorityQueue(queue, createHuffmanNode(data[i], freq[i]));
    }

    // Construct the Huffman tree using the nodes in the priority queue.
    while (queue->size > 1) {
        // Extract the two nodes with the smallest frequencies from the queue.
        left = extractMin(queue); // Node with smallest frequency
        right = extractMin(queue); // Node with second smallest frequency

        // Create a new internal node. The '$' is a placeholder symbol for internal nodes.
        // The frequency of this new node is the sum of the frequencies of 'left' and 'right'.
        top = createHuffmanNode('$', left->freq + right->freq);

        // Set the extracted nodes as children of the new node.
        top->left = left;
        top->right = right;

        // Insert the new internal node back into the priority queue.
        insertSortedArrayPriorityQueue(queue, top);
    }

    // The last remaining node in the priority queue is the root of the Huffman tree.
    return extractMin(queue);
}


// Helper function to store Huffman codes in a map/array
void storeCodes(struct HuffmanNode* root, char* code, int top, char codes[256][256]) {
    // Store the code for leaf nodes (characters)
    if (root->left == NULL && root->right == NULL && isalpha(root->data)) {
        code[top] = '\0';
        strcpy(codes[(unsigned char)root->data], code);
        return;
    }

    // in a recusrive way If we move left, add '0' to the code
    if (root->left) {
        code[top] = '0';
        storeCodes(root->left, code, top + 1, codes);
    }

    // in a recursive way If we move right, add '1' to the code
    if (root->right) {
        code[top] = '1';
        storeCodes(root->right, code, top + 1, codes);
    }
}

// Function to generate and return Huffman codes
void generateHuffmanCodes(struct HuffmanNode* root, char codes[256][256]) {
    char code[256];
    storeCodes(root, code, 0, codes);
}

// Function to encode the input data using Huffman codes
char* encodeData(const char* data, char codes[256][256]) {
    // Allocate memory to store the encoded data. 
    // We allocate enough space considering the worst-case scenario where each character in the data is replaced by a Huffman code of maximum length.
    char* encodedData = (char*)malloc(strlen(data) * 256);
    // Check if memory allocation was successful, return NULL if not.
    if (!encodedData) return NULL;

    // Initialize the first character of the encoded data string to the null character
    encodedData[0] = '\0';

    // Iterate over each character in the input data.
    for (int i = 0; data[i] != '\0'; i++) {
        // Concatenate the Huffman code of the current character to the encoded data string 'codes' is a 2D array where each character's ASCII value indexes its Huffman code.
        strcat(encodedData, codes[(unsigned char)data[i]]);

        // After adding each code, append a space to serve as a delimiter This makes it easier to identify individual codes in the encoded string.
        strcat(encodedData, " ");
    }

    // Return the fully encoded data string.
    return encodedData;
}

// Function to decode the encoded data using the Huffman tree
char* decodeData(struct HuffmanNode* root, const char* encodedData) {
    // Allocate memory for the decoded data. Its size is the same as the encoded data
    // since the decoded output will be equal to or shorter than the encoded input.
    char* decodedData = (char*)malloc(strlen(encodedData));
    // If memory allocation fails, return NULL.
    if (!decodedData) return NULL;

    // Pointer to navigate and build the decoded data string.
    char* decodedPtr = decodedData;
    // Start from the root of the Huffman tree.
    struct HuffmanNode* currentNode = root;

    // Iterate through each character in the encoded data.
    for (int i = 0; encodedData[i] != '\0'; i++) {
        // Move left or right in the Huffman tree based on the current character ('0' or '1').
        if (encodedData[i] == '0') {
            currentNode = currentNode->left;
        }
        else if (encodedData[i] == '1') {
            currentNode = currentNode->right;
        }

        // Check if a leaf node is reached or a space delimiter is encountered.
        if ((currentNode->left == NULL && currentNode->right == NULL) || encodedData[i] == ' ') {
            // Add the character at the leaf node to the decoded string.
            *decodedPtr++ = currentNode->data;
            // Reset to start from the root for the next character.
            currentNode = root;

            // If the next character is a space (a delimiter), skip it.
            if (encodedData[i + 1] == ' ') {
                i++;
            }
        }
    }

    // Null-terminate the decoded string.
    *decodedPtr = '\0';
    // Return the decoded data.
    return decodedData;
}

// Function to print Huffman codes for each character
void printCodes(char codes[256][256]) {
    // Print a header line to indicate the start of Huffman codes.
    printf("Huffman Codes:\n");

    // Loop through each possible character (0 to 255) in the ASCII table.
    for (int i = 0; i < 256; i++) {
        // Check if a Huffman code exists for the character with ASCII value 'i'The condition checks the first character of the Huffman code string for character 'i'.
        // If the first character is not the null character '\0', it means a Huffman code exists for this character.
        if (codes[i][0] != '\0') {
            // Print the character and its corresponding Huffman code.
            printf("%c: %s\n", (char)i, codes[i]);
        }
    }
}


// Function to recursively free the memory allocated for the Huffman tree
void freeHuffmanTree(struct HuffmanNode* root) {
    if (root == NULL) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}


// Function to encode text from a file using Huffman codes.
char* encodeTextFromFile(struct HuffmanNode* root, char codes[256][256], const char* inputFilename) {
    // Open the input file in read mode.
    FILE* inputFile = fopen(inputFilename, "r");
    // Check if the file opening was successful.
    if (!inputFile) {
        // Print an error message if the file cannot be opened.
        perror("Error opening input file");
        // Return NULL to indicate an error occurred.
        return NULL;
    }

    // Move the file pointer to the end of the file to determine its size.
    fseek(inputFile, 0, SEEK_END);
    // Retrieve the size of the file.
    long fileSize = ftell(inputFile);
    // Move the file pointer back to the start of the file.
    rewind(inputFile);

    // Allocate memory for storing the entire content of the file.
    char* fileContent = (char*)malloc(fileSize + 1);
    // Check if memory allocation was successful.
    if (!fileContent) {
        // Print an error message if memory allocation fails.
        perror("Memory allocation for file content failed");
        // Close the input file before returning.
        fclose(inputFile);
        // Return NULL to indicate an error occurred.
        return NULL;
    }

    // Read the entire content of the file into the allocated buffer.
    fread(fileContent, fileSize, 1, inputFile);
    // Add a null character at the end of the buffer to terminate the string.
    fileContent[fileSize] = '\0';
    // Close the input file as it's no longer needed.
    fclose(inputFile);

    // Encode the content of the file using Huffman codes.
    char* encodedData = encodeData(fileContent, codes);
    // Free the memory allocated for the file content.
    free(fileContent);

    // Return the encoded data.
    return encodedData;
}


int main() {
    // Letter frequencies for English and French, scaled by 100 to avoid floating points
    char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMOPQRSTUVWXYZ";
    unsigned english_freq[] = {
        834, 154, 273, 414, 1260, 203, 192, 611, 671, 23,
        87, 424, 253, 680, 770, 166, 9, 568, 611, 937,
        285, 106, 234, 20, 204, 6, 834, 154, 273, 414, 1260, 203, 192, 611, 671, 23,
        87, 424, 253, 680, 770, 166, 9, 568, 611, 937,
        285, 106, 234, 20, 204, 6
    };
    unsigned french_freq[] = {
        813, 93, 315, 355, 1510, 96, 97, 108, 694, 71,
        16, 568, 323, 642, 527, 303, 89, 643, 791, 711,
        605, 183, 4, 42, 19, 106, 813, 93, 315, 355, 1510, 96, 97, 108, 694, 71,
        16, 568, 323, 642, 527, 303, 89, 643, 791, 711,
        605, 183, 4, 42, 19, 106
    };

    // Size of the letters array
    int size = sizeof(letters) / sizeof(letters[0]);

    // Build Huffman trees for English and French
    struct HuffmanNode* english_root = buildHuffmanTree(letters, english_freq, size);
    struct HuffmanNode* french_root = buildHuffmanTree(letters, french_freq, size);

    // Generate Huffman codes for English and French
    char english_codes[256][256] = { 0 };
    char french_codes[256][256] = { 0 };
    generateHuffmanCodes(english_root, english_codes);
    generateHuffmanCodes(french_root, french_codes);

    // Print Huffman codes for English
    printf("English Huffman Codes:\n");
    printCodes(english_codes);

    // Print Huffman codes for French
    printf("\nFrench Huffman Codes:\n");
    printCodes(french_codes);

    // Assume we have text files for English and French input
    char englishInputFilename[] = "english_input.txt";
    char frenchInputFilename[] = "french_input.txt";

    // Encode and Decode English text
    char* english_encodedData = encodeTextFromFile(english_root, english_codes, englishInputFilename);
    printf("\nEncoded English Text:\n%s\n", english_encodedData);
    char* english_decodedData = decodeData(english_root, english_encodedData);
    printf("\nDecoded English Text:\n%s\n", english_decodedData);

    // Free encoded and decoded English text
    free(english_encodedData);
    free(english_decodedData);

    // Encode and Decode French text
    char* french_encodedData = encodeTextFromFile(french_root, french_codes, frenchInputFilename);
    printf("\nEncoded French Text:\n%s\n", french_encodedData);
    char* french_decodedData = decodeData(french_root, french_encodedData);
    printf("\nDecoded French Text:\n%s\n", french_decodedData);

    // Free encoded and decoded French text
    free(french_encodedData);
    free(french_decodedData);

    // Clean up memory for both Huffman trees
    freeHuffmanTree(english_root);
    freeHuffmanTree(french_root);

    return 0;
}