import re
import sys
from subprocess import call

# Swap all bits from LSB to MSB
def bitReverse(input):
    output = input;

    output = (output & 0xF0) >> 4 | (output & 0x0F) << 4;
    output = (output & 0xCC) >> 2 | (output & 0x33) << 2;
    output = (output & 0xAA) >> 1 | (output & 0x55) << 1;

    return output;

# Reverse the bits in a hex string byte
def reverseByte(text):
    index = text.find("0x")
    
    # No hex value found
    if index == -1:
        return text
    
    # Get the hex string
    hexStr = text[index:index+4]
    
    # Reverse the bits in the byte
    rev = bitReverse(int(hexStr, 16))
    
    # Convert back to string
    text = text[:index] + decToHexStr(rev)
    return text
    
# Format integer as hex byte string
def decToHexStr(value):
    return "0x{0:02X}".format(value)
    
def main():
    # Validate command line arguments
    if len(sys.argv) < 2:
        print("Invalid arguments!\n")
        print("Usage:")
        print("python convert.py <image_file>")
        input("Press enter to continue...")
        sys.exit()

    for i in range(1, len(sys.argv)):
        # Get file name from command line argument
        filename = str.split(sys.argv[i], '.')[0] + '.h'

        # Convert to XBM using ImageMagick
        print('"C:\Program Files\ImageMagick-6.8.9-Q16\convert.exe" "' + sys.argv[i] + '" XBM:"' + filename + '"')
        call('"C:\Program Files\ImageMagick-6.8.9-Q16\convert.exe" "' + sys.argv[i] + '" XBM:"' + filename + '"')
        
        # Read XBM file into string
        with open(filename, 'r') as file:
            text = file.read()
        
        # Use Windows line endings
        text.replace('\n', "\r\n")
        
        # Find the array definition
        start = text.find('{')
        stop = text.rfind('}')
        
        # Get the array string without the curly brackets
        # and split by bytes
        array = text[start+1:stop].split(',')

        # Remove extra comma from array list
        if array[-1].isspace():
            array.remove(array[-1])
        
        # Reverse each byte
        for i in range(0, len(array)):
            if i < len(array) - 1:
                array[i] = reverseByte(array[i]) + ','
            else:
                array[i] = reverseByte(array[i])
        
        # Convert array back into a single string
        array = str.join('', array)
        
        # Replace new array
        text = text[:start+1] + array + text[stop:]
        
        # Make code constant
        text = re.sub(r'static char ([a-zA-Z0-9_]*)\[\] = \{', r'static SI_SEGMENT_VARIABLE(\1[], const uint8_t, SI_SEG_CODE) = {', text)
        
        # Overwrite file
        with open(filename, 'w') as file:
            file.write(text)
    
if __name__ == "__main__":
    main()
