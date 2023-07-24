# Hive BSQ

Just a quick repo for my solving prototype at hive during the bsq.
The task was to find the largest square in a rectangular map filled with either empty "." or obstacle "o" tiles.
Then draw the largest square in the map with "x" 's. The symbols could be anything from user input.


This is not the final version of the solver but the ideas inside here are good in my opinion.
There are a few parts where speed can be improved, but the actual solving part works at an acceptable speed.
Two obvious improvements for speed would be
 - Keep largest square size while moving in yx grid instead of always starting at size 0.
    This allows you to skip several loops when the square size starts becoming larger
 - Stop the loop if next yx position + square size is out of bounds.
    This can potentially cut the time by more than half if the square is found early and is over a quarter of the map size.
