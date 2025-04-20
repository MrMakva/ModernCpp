package checks

import "path/filepath"

func CollectProjectFiles(projectDir, pattern string) ([]string, error) {
	projectFiles, err := filepath.Glob(filepath.Join(projectDir, pattern))
	if err != nil {
		return nil, err
	}
	projectFileNames := make([]string, 0, len(projectFiles))
	for _, projectFile := range projectFiles {
		projectFileNames = append(projectFileNames, filepath.Base(projectFile))
	}
	return projectFileNames, nil
}
