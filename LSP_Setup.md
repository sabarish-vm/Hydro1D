# Set up compile_commands.json for better LSP functionality

Run the following :

```
cmake -S . -G "Unix Makefiles" -B cmake
ln -s cmake/compile_commands.json .
```
