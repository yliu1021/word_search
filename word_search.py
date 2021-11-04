import asyncio
from subprocess import list2cmdline
import sys
from typing import Optional, Union


class WordSearch:
    def __init__(self, path_to_word_search, path_to_corpus):
        self.word_search_path = path_to_word_search
        self.corpus = path_to_corpus
        self.proc: Optional[asyncio.subprocess.Process] = None

    async def __aenter__(self) -> "WordSearch":
        self.proc = await asyncio.create_subprocess_exec(
            self.word_search_path,
            self.corpus,
            stdin=asyncio.subprocess.PIPE,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.STDOUT
        )
        while True:
            line = await self.proc.stdout.readline()
            line = line.decode().rstrip()
            if line.startswith("Reading") or line.startswith("Unable to insert") or line.startswith("Read"):
                continue
            elif line == "Loaded corpus":
                break
            else:
                raise RuntimeError(f"Word search reported error: {line}")
        return self

    async def __aexit__(self, exc_type, exc_val, exc_tb):
        self.proc.stdin.write_eof()
        await self.proc.stdin.drain()
        await self.proc.wait()

    async def get_words(self, grid: Union[str, list[list[str]], list[str]]) -> list[list[tuple[str, tuple[int, int]]]]:
        lines = await self._get_words_raw(grid)

        def parse_char_pos(char_pos: str) -> tuple[str, tuple[int, int]]:
            char, pos = char_pos.split(":")
            x, y = pos.split(",")
            x = int(x)
            y = int(y)
            return char, (x, y)

        def parse_line(line) -> list[tuple[str, tuple[int, int]]]:
            word_chars = line.split()
            return [parse_char_pos(char_pos) for char_pos in word_chars]

        return [parse_line(line) for line in lines]

    async def _get_words_raw(self, grid: Union[str, list[list[str]], list[str]]) -> list[str]:
        if not isinstance(grid, str):
            grid = ["".join(row) if isinstance(row, list) else row for row in grid]
            grid = "\n".join(grid)
        grid = grid.encode("ascii")
        self.proc.stdin.write(grid)
        self.proc.stdin.write(b"\n\n")
        await self.proc.stdin.drain()
        out_lines = []
        while True:
            line = await self.proc.stdout.readline()
            line = line.decode().rstrip()
            if line == "-":
                break
            elif line == "invalid grid":
                raise ValueError(f"Invalid grid: {grid}")
            else:
                out_lines.append(line)
        return out_lines

    async def get_lists_of_words_from_grid(self, words_in_grid, word_grid: list[str]) -> list[list]:
        all_words = list()

        for word in words_in_grid:
            print(word)
            
            new_grid = word_grid.copy()

            new_grid = await self.collapse_word_grid()
                
            print_grid(new_grid)

            new_words_in_grid = await self.get_words(new_grid)

            if new_words_in_grid:
                this_list = [word] + await self.get_lists_of_words_from_grid(words_in_grid = new_words_in_grid, word_grid = new_grid)
            
                all_words.append(this_list)
            
            else:
                all_words.append([word])

        return all_words

    async def collapse_word_grid(word, new_grid) -> None:
        for letter in word:
            row, col = letter[1]
            if row == 0:
                new_grid[row] = new_grid[row][:col] + ' ' + new_grid[row][col+1:]
            else:
                row_i = row
                while(row_i != 0):
                    new_grid[row_i] = new_grid[row_i][:col] + new_grid[row_i-1][col] + new_grid[row_i][col+1:] 
                    row_i -= 1
                new_grid[row_i] = new_grid[row_i][:col] + ' ' + new_grid[row_i][col+1:] 

    async def update_letter_locations(word, row, col):
        # This function needs to go in the while loop updating the letter's coordinates to be accurate.
        pass
            

def print_grid(word_grid: list[str]):
    for row in word_grid:
        print(row)
    print()

async def main(word_search_loc, corpus_loc):
    word_search = WordSearch(word_search_loc, corpus_loc)
    async with word_search:
        word_grid = [
            "abc",
            "def",
            "..g"
        ]
        
        #words_in_grid = await word_search.get_words(word_grid)
        #list_of_words = await word_search.get_lists_of_words_from_grid(words_in_grid=words_in_grid, word_grid=word_grid)
        #print(list_of_words)
        

if __name__ == "__main__":
    if len(sys.argv) == 2:
        print("usage: python word_search.py [PATH_TO_WORD_SEARCH] [PATH_TO_CORPUS]")
        exit(1)
    asyncio.run(main(sys.argv[1], sys.argv[2]))
