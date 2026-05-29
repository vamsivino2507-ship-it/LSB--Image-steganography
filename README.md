🔐 LSB Image Steganography in C
A console-based Steganography application written in C that hides secret files inside BMP images using the Least Significant Bit (LSB) technique — and can decode them back. The hidden data is invisible to the naked eye.

📋 Features

Encode — Embed any secret file (e.g., .txt) into a BMP image
Decode — Extract the hidden file from a stego image
Validation — Checks argument formats, file extensions, and image capacity
Magic String — Verifies authenticity of the stego image before decoding
Persistence — Output is a new BMP image; the original is never modified


🛠️ Project Structure
steganography/
├── main.c          # Entry point; detects -e / -d operation and dispatches
├── encode.c        # All encoding logic (LSB embedding, capacity check, etc.)
├── encode.h        # EncodeInfo struct and encoding function declarations
├── decode.c        # All decoding logic (LSB extraction, magic string verify)
├── decode.h        # DecodeInfo struct and decoding function declarations
├── types.h         # Shared types: uint, Status, OperationType enums
├── decodetype.h    # Dstatus enum for decode-specific return values
└── common.h        # Shared constants (MAGIC_STRING, etc.)

🔬 How It Works
Encoding
Each byte of the secret file is split into 8 bits and embedded into the least significant bit of 8 consecutive bytes of the BMP image's pixel data. This changes pixel values by at most 1 — imperceptible to the human eye.
The stego image stores data in this order:
[BMP Header (54 bytes)] [Magic String] [Extension Size] [Extension] [File Size] [File Data] [Remaining Image]
Decoding
The decoder reads the stego image, verifies the magic string, then reconstructs the extension size, extension, file size, and file content by reading LSBs in the same order.

✅ Input Validation
CheckRuleSource imageMust be a .bmp fileOutput imageMust be a .bmp file (defaults to stego_img.bmp)Secret fileMust have a valid extension (e.g., .txt)CapacityBMP image must have enough pixels to hold the secret fileMagic stringDecode only proceeds if magic string matches

🚀 Getting Started
Prerequisites

GCC compiler
Linux / macOS / Windows with MinGW

Compilation
bashgcc main.c encode.c decode.c -o steganography -lm
Usage
Encode (hide a secret file into a BMP):
bash./steganography -e <source.bmp> <secret.txt> [output.bmp]
Decode (extract hidden file from stego image):
bash./steganography -d <stego.bmp> [output_filename]

📌 Examples
bash# Encode secret.txt into beautiful.bmp → stego_img.bmp (default output)
./steganography -e beautiful.bmp secret.txt

# Encode with a custom output filename
./steganography -e beautiful.bmp secret.txt result.bmp

# Decode from stego image (output file name auto-detected from extension)
./steganography -d stego_img.bmp

# Decode with a custom output filename (extension auto-appended)
./steganography -d stego_img.bmp decoded_output

⚙️ Capacity Formula
The BMP image must satisfy:
image_capacity > 54 + 16 + 32 + 32 + 32 + (secret_file_size × 8)  [in bits]
Where image_capacity = width × height × 3 bytes (RGB pixels).

👤 Author
Vamsi D
Batch No: 25039_005
