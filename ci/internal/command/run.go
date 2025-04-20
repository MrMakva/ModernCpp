package command

import (
	"log"
	"os/exec"
)

type CommandResult struct {
	ExitStatus int
	Output     string
}

func RunMake() CommandResult {
	cmdName := "make"
	cmd := exec.Command(cmdName, `CXXFLAGS=-Wall -Wextra -Werror -pedantic -std=c++20`)
	log.Println(cmdName)
	output, _ := cmd.CombinedOutput()
	return CommandResult{ExitStatus: cmd.ProcessState.ExitCode(), Output: string(output)}
}
