import asyncio
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


async def main(word_search_loc, corpus_loc):
    word_search = WordSearch(word_search_loc, corpus_loc)
    async with word_search:
        res = await word_search.get_words([
            "abc",
            "def",
            "..g"
        ])
        print(res)
        res = await word_search.get_words([
            "aa",
            "bb"
        ])
        print(res)


if __name__ == "__main__":
    if len(sys.argv) == 2:
        print("usage: python word_search.py [PATH_TO_WORD_SEARCH] [PATH_TO_CORPUS]")
        exit(1)
    asyncio.run(main(sys.argv[1], sys.argv[2]))
