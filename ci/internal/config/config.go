package config

type App struct {
	Tasks []Task `yaml:"tasks"`
}

type Task struct {
	Name            string   `yaml:"name"`
	Pattern         string   `yaml:"pattern"`
	Project         []File   `yaml:"project"`
	Artifacts       []string `yaml:"artifacts"`
	ClangTidyConfig string   `yaml:"clang_tidy_config"`
}

type File struct {
	Path     string `yaml:"path"`
	Optional bool   `yaml:"optional"`
}
