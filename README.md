# Word Search

Word search is a program that takes a corpus and searches for those words in a grid of letters using a trie and DFS.

## Quick start

Word search requires a corpus of words (a text file of words in lowercase with only letters and each word on a new line)
. You can download one [here](http://www.mieliestronk.com/corncob_lowercase.txt). To build and run word search, run the
following:

```
> mkdir build
> cd build
> cmake ..
> make
> ./src/word_search path/to/corpus.txt
```

If you downloaded the corpus from the above link, you should see the output

```
Reading ./corpora/english_corpus_corn_cob.txt
Unable to insert "cross-bun" into trie (line no. 11652)... ignoring
Read 58110 lines in 0.128 secs
Loaded corpus
```

*(note that "cross-bun" cannot be loaded because it contains a dash "-")*

Once loaded, simply enter a grid as a list of rows separated by new lines.

```
cake..
.a....
..t...
```

Note that you can use '.' or any non-lowercase letter to denote a whitespace. Once you've finished entering the grid,
press enter again to insert an empty line and word search will list all the words that it's found

```
cake..
.a....
..t...

a:1,1 t:2,2 
c:0,0 a:1,1 t:2,2 
c:0,0 a:0,1 k:0,2 e:0,3 
c:0,0 a:1,1 k:0,2 e:0,3 
t:2,2 a:1,1 k:0,2 e:0,3
```

Each word will be printed on a new line as a space delimited list of `[char]:[pos]` tokens. The position is reported
as `[row],[col]` where the top left corner of the grid is (0, 0). In the above example, the first word is "at", with
letter 'a' at position (1, 1) and 't' at position (2, 2).

To exit, either send a `Ctrl-C` or `Ctrl-D`.

## Internals

Word search internally uses a trie to perform a DFS at each location of the grid. It will efficiently disregard paths
that are not a prefix to any word in the corpus. Since the longest english word is bounded to only a few letters (in the
above link it's 23), the depth of the search is bounded by a constant.
