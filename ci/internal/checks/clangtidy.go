package checks

import (
	"os/exec"

	"cpp/ci/internal/config"
)

func RunClangTidy(projectDir string, task *config.Task) CheckOutput {
	sourceFiles, _ := CollectProjectFiles(projectDir, "*.cpp")

	outputLines := make([]string, 0)
	for _, sourceFile := range sourceFiles {
		cmd := exec.Command("clang-tidy", "--config-file", task.ClangTidyConfig, sourceFile, "--", "-std=c++20")
		output, err := cmd.CombinedOutput()
		if err != nil {
			if len(output) > 0 {
				outputLines = append(outputLines, string(output))
			} else {
				outputLines = append(outputLines, err.Error())
			}
		}
	}

	return CheckOutput{
		Success:   len(outputLines) == 0,
		CheckName: "Clang Tidy",
		Details:   outputLines,
	}
}
