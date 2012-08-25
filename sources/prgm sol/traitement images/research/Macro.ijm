run("Split Channels");
imageCalculator("Add create 32-bit", "IMG01.JPG (red)","IMG01.JPG (green)");
selectWindow("IMG01.JPG (blue)");
run("32-bit");
run("Multiply...", "value=2.000");
imageCalculator("Subtract create 32-bit", "Result of IMG01.JPG (red)","IMG01.JPG (blue)");
imageCalculator("Add create 32-bit", "Result of IMG01.JPG (red)","IMG01.JPG (blue)");
imageCalculator("Divide create 32-bit", "Result of Result of IMG01.JPG (red)","Result of Result of IMG01.JPG (red)");
selectWindow("Result of Result of Result of IMG01.JPG (red)");
