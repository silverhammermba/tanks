require 'rake/clean'

CLEAN.include('*.o')

task :default => 'launch'

file 'launch' do
	sh "g++ -ggdb -o launch #{FileList['*.o']} -lyaml-cpp -lBox2D -lsfml-graphics -lsfml-window -lsfml-system"
end

file 'engine.o' => ['engine.cpp', *FileList['*.hpp']] do |t|
	sh "g++ -ggdb -std=c++11 -c -o #{t.name} #{t.name.ext('.cpp')}"
	file 'launch' => t.name
end

(FileList['*.cpp'] - ['engine.cpp']).each do |name|
	obj = name.ext('.o')
	file obj => [name, 'engine.o'] do
		sh "g++ -ggdb -std=c++11 -c -o #{obj} #{name}"
	end
	file 'launch' => obj
end
