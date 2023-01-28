# AccSaber Quest Plugin

## Reporting Issues
* The best way to report issues is to click on the `Issues` tab at the top of the GitHub page. This allows any contributor to see the problem and attempt to fix it, and others with the same issue can contribute more information. **Please try the troubleshooting steps before reporting the issues listed there. Please only report issues after using the latest build, your problem may have already been fixed.**
* Include in your issue:
  * A detailed explanation of your problem (you can also attach videos/screenshots)
  * **Important**: The log file from the game session the issue occurred (restarting the game creates a new log file).
* If you ask for help on Discord, at least include your log file in your help request.

## Contributing
Anyone can feel free to contribute bug fixes or enhancements to the AccSaber plugin! Fork, make your changes and pull request it!
### Building
Visual Studio Code with the C/C++ Extension pack is the recommended developing environment!
1. Check out the repository
2. Open the `AccSaber/src` folder
3. Open a terminal and run `qpm-rust restore`
    * This assumes you already have qpm-rust, CMake and ninja-build already installed and on path.
4. The project should now build using `pwsh .\buildQMOD.ps1`.
