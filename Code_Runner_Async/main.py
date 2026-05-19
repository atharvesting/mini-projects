import subprocess
import os

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

print(dir(os.path))

