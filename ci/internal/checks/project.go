package checks

import (
	"strings"

	"cpp/ci/internal/config"
)

const checkName = "Project structure"

func CheckProjectStructure(projectDir string, task *config.Task) CheckOutput {
	expectedFiles := task.Project
	projectFiles, _ := CollectProjectFiles(projectDir, "*")

	missingFiles := collectMissingFiles(projectFiles, expectedFiles)
	extraFiles := collectExtraFiles(expectedFiles, projectFiles)

	if len(missingFiles) == 0 && len(extraFiles) == 0 {
		return CheckOutput{Success: true, CheckName: checkName}
	}

	details := make([]string, 0)

	var missingFilesText string
	if len(missingFiles) > 0 {
		missingFilesText = "Missing files: " + strings.Join(missingFiles, ", ")
		details = append(details, missingFilesText)
	}

	var extraFilesText string
	if len(extraFiles) > 0 {
		extraFilesText = "Extra files: " + strings.Join(extraFiles, ", ")
		details = append(details, extraFilesText)
	}

	return CheckOutput{
		Success:   false,
		CheckName: checkName,
		Details:   details,
	}
}

func collectExtraFiles(expectedFiles []config.File, projectFiles []string) []string {
	expectedFilesSet := map[string]bool{}
	for _, expectedFile := range expectedFiles {
		expectedFilesSet[expectedFile.Path] = true
	}
	extraFiles := make([]string, 0)
	for _, projectFile := range projectFiles {
		if !expectedFilesSet[projectFile] && projectFile != ".git" {
			extraFiles = append(extraFiles, projectFile)
		}
	}
	return extraFiles
}

func collectMissingFiles(projectFiles []string, expectedFiles []config.File) []string {
	projectFilesSet := map[string]bool{}
	for _, projectFile := range projectFiles {
		projectFilesSet[projectFile] = true
	}
	missingFiles := make([]string, 0)
	for _, expectedFile := range expectedFiles {
		if !projectFilesSet[expectedFile.Path] && !expectedFile.Optional {
			missingFiles = append(missingFiles, expectedFile.Path)
		}
	}
	return missingFiles
}
