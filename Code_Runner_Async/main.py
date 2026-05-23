import subprocess
import os
import config

"""
subprocess.run('ls -l', shell=True)                                         Uses shell
subprocess.run(['ls', '-l'])                                                Avoids shell
t = subprocess.run(['ls', '-l'], capture_output=True); print(t.stdout)      Not a string
t = subprocess.run(['ls', '-l'], capture_output=True, text=True)            Is a string
t = subprocess.run(['ls', '-l'], stdout=subprocess.PIPE, text=True)         capture_output abstracts this step

# Redirect stdout to a file
with open("output.txt", 'w') as f:
    t = subprocess.run(['ls', '-l'], stdout=f, text=True)
    
# Make Python throw an exception if subprocess command returns non-zero code
t = subprocess.run(['ls', '-l', 'dne'], capture_output=True, text=True, check=True)
print(t.stderr)

# Grab output from one command and use it in another
t = subprocess.run(['cat', 'README.md'], capture_output=True, text=True)
g = subprocess.run(['rg' '-n', 'Vibe-coded'], capture_output=True, text=True,
                   input=t.stdout)
print(g.stdout)

"""

src_filename = "test.cpp"
exe_filename = "output.exe"
compiler = config.COMPILER_PATH

full_src_path = os.path.join(os.getcwd(), src_filename)
full_exe_path = os.path.join(os.getcwd(), exe_filename)

# current_path = subprocess.run(['pwd'], shell=True, capture_output=True, text=True)
# subprocess.run(['cd', current_path.stdout], shell=True)
# subprocess.run(['ls'])

if os.path.exists(full_exe_path):
    subprocess.run(['rm', full_exe_path])

a = subprocess.run([compiler, full_src_path, '-o', full_exe_path], capture_output=True, text=True)
# print(a.returncode)
# print(a.stdout)
# print(a.stderr)

b = subprocess.run(['output.exe'], capture_output=True, text=True)

print("Return code:", b.returncode)
print("Output:\n", b.stdout)
print("Errors:", b.stderr)

