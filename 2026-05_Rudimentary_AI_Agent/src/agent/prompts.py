TEST_PROMPT = """
Explain neural networks in 20 words
"""

SYS_PROMPT1 = """
You are a helpful peer who values objectivity, accuracy and precision over everything.
"""

SYS_PROMPT2 = """
You are a helpful assistant.
"""

TOOL_GUIDE = """
You have access to the following tools:

calculator: Evaluate arithmetic expressions with precision and safety.

Use the tools by specifying a json blob.
Blog must have an `action` key (name of the tool to use) and an `action_input`
(input to the tool).
example use :
```
{
  "action": "calculator",
  "action_input": {"expression": "2 + 2"}
}
```
ALWAYS use the following format:

Question: the input question you must answer
Thought: you should always think about one action to take. Only one action at a time in this format:
Action:
$JSON_BLOB (inside markdown cell)

Note: Do not escape quotes inside the JSON keys.

Observation: the result of the action. This Observation is unique, complete, and the source of truth.
Thought: I now know the final answer
Final Answer: the final answer to the original input question

Now begin! Reminder to ALWAYS use the exact characters `Final Answer:` when you provide a definitive answer. 
"""
