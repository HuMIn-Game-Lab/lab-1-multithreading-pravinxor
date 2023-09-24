import subprocess
import json
import re

def compile_with_clang(directive):
    command = ['make', directive]
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()

    errors = stderr.decode('utf-8').split('\n')
    errors_json = []
    error_data = {}
    for error in errors:
        if error:
            if re.match(r'.*error:.*', error):
                if error_data:
                    errors_json.append(error_data)
                match = re.match(r'(.*):(\d+):(\d+): error: (.*)', error)
                if match:
                    file, line, column, message = match.groups()
                    error_data = {
                        'file': file,
                        'line': int(line),
                        'column': int(column),
                        'message': message.strip(),
                    }

    if error_data:
        errors_json.append(error_data)

    return errors_json

errors_json = compile_with_clang('demo')
print(json.dumps(errors_json, indent=4))

