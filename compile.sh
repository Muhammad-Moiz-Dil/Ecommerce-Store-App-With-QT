#!/bin/bash

# Make sure the moc output directory exists
mkdir -p gui/moc

# Generate moc files from Qt headers
echo "Generating moc files from headers..."
for header in gui/include/*.h; do
  if [ -f "$header" ]; then
    base_name=$(basename "$header" .h)
    # Generate moc files in the gui/moc directory
    output_file="gui/moc/moc_${base_name}.cpp"
    echo "Processing $header -> $output_file"
    /Users/macbookpro/Qt/6.9.0/macos/libexec/moc "$header" -o "$output_file"
  fi
done

# List of source files 
SOURCE_FILES="main.cpp"
# Add all generated moc files
for mocfile in gui/moc/moc_*.cpp; do
  if [ -f "$mocfile" ]; then
    SOURCE_FILES+=" $mocfile"
  fi
done
# Add other source files
for file in gui/src/*.cpp; do
  if [ -f "$file" ]; then
    SOURCE_FILES+=" $file"
  fi
done

# Compile the application using the commands from path.md
echo "Compiling application..."
g++ -std=c++17 -fPIC \
    -Igui/include \
    -Isrc \
    -I. \
    -I/Users/macbookpro/Qt/6.9.0/macos/lib/QtWidgets.framework/Versions/A/Headers \
    -I/Users/macbookpro/Qt/6.9.0/macos/lib/QtGui.framework/Versions/A/Headers \
    -I/Users/macbookpro/Qt/6.9.0/macos/lib/QtCore.framework/Versions/A/Headers \
    $SOURCE_FILES \
    -F/Users/macbookpro/Qt/6.9.0/macos/lib \
    -framework QtWidgets \
    -framework QtGui \
    -framework QtCore \
    -Wl,-rpath,/Users/macbookpro/Qt/6.9.0/macos/lib \
    -o ECommerceApp_Qt

if [ $? -eq 0 ]; then
    echo "Compilation successful! Run the application with ./ECommerceApp_Qt"
else
    echo "Compilation failed."
fi 