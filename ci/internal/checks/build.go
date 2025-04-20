package checks

import (
	"strings"

	"cpp/ci/internal/command"
	"cpp/ci/internal/config"
)

func CheckBuild(projectDir string, task *config.Task) CheckOutput {
	cmdResult := command.RunMake()
	return CheckOutput{
		Success:   cmdResult.ExitStatus == 0,
		CheckName: "Build",
		Details:   strings.Split(cmdResult.Output, "\n"),
	}
}
