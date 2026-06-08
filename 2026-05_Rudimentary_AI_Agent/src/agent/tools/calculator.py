from pydantic import BaseModel
import ast
import operator
from agent.tools.base_tool import Tool

class Calculator(Tool):

    def __init__(self):
        self.name = "Calculator"
        self.description = "Evaluate mathematical expressions with precision and safety."
        self.func = self.calculate
        self.arguments = [("expression", "str")]
        self.outputs = "int | float"

    SAFE_OPERATORS = {
        ast.Add: operator.add,
        ast.Sub: operator.sub,
        ast.Mult: operator.mul,
        ast.Div: operator.truediv,
        ast.Pow: operator.pow,
        ast.USub: operator.neg
    }

    @staticmethod
    def evaluate_expr(node):
        if isinstance(node, ast.Constant):
            return node.value
        
        elif isinstance(node, ast.BinOp):
            left = Calculator.evaluate_expr(node.left)
            right = Calculator.evaluate_expr(node.right)
            return Calculator.SAFE_OPERATORS[type(node.op)](left, right)
        
        elif isinstance(node, ast.UnaryOp):
            operand = Calculator.evaluate_expr(node.operand)
            return Calculator.SAFE_OPERATORS[type(node.op)](operand)
        
        else:
            raise TypeError(f"Unsupported node type: {type(node)}")
        
    def calculate(self, expression: str):
        node = ast.parse(expression, mode='eval').body
        return Calculator.evaluate_expr(node)