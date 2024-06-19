import xml.etree.ElementTree as ET
import sys


def process_tile_info(filename, output_filename):
    """
    This function reads a tile information file in XML format, extracts the ID
    and the last part of the source, and writes them next to each other in a line.

    Args:
        filename: The path to the XML file containing tile information.
    """

    tree = ET.parse(filename)
    root = tree.getroot()

    with open(output_filename, 'w') as output_file:
        for tile in root.findall('tile'):
            # Extract ID
            tile_id = tile.attrib['id']

            # Extract the last part of the source (assuming path format)
            source = tile.find('image').attrib['source']
            # Split by '/' and get last element
            source_last_part = source.split('/')[-1]

            # Write ID and source next to each other
            output_line = f"{tile_id} {source_last_part}\n"
            output_file.write(output_line)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: ./your_python_script_name your_xml_file")
        exit(1)

    # Replace 'your_file.xml' with the actual filename
    filename = sys.argv[1]
    output_filename = 'mapper'

    process_tile_info(filename, output_filename)
