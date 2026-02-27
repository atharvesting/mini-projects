import json
from pathlib import Path
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn import tree
from sklearn.ensemble import RandomForestClassifier
from sklearn import svm
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import classification_report, confusion_matrix
from imblearn.over_sampling import SMOTE

# =========================
# DATA LOADING & PREP
# =========================
data = pd.read_csv("data_2.csv")
data.dropna(inplace=True)

X_labels = ["B1", "B2", "B3", "B4", "B5", "B6", "B7", "NDVI", "NDWI", "NDBI"] # Filtering out coordinates
X = data[X_labels]
y = data["class"]

X_train, X_test, y_train, y_test = train_test_split(
    X, y, random_state=1, test_size=0.2
)

smote = SMOTE(random_state=1)
X_train_balanced, y_train_balanced = smote.fit_resample(X_train, y_train)

# Scale only for models that need it (e.g., SVM)
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train_balanced)
X_test_scaled = scaler.transform(X_test)

# =========================
# OUTPUT SETUP
# =========================
results_dir = Path("results")
results_dir.mkdir(exist_ok=True)

summary_rows = []

def evaluate_and_save(model_name, y_true, y_pred):
    """Save report + confusion matrix for one model."""
    report_dict = classification_report(y_true, y_pred, output_dict=True)
    report_text = classification_report(y_true, y_pred)
    cm = confusion_matrix(y_true=y_true, y_pred=y_pred)

    print(f"\n===== {model_name} =====")
    print(report_text)
    print(cm)

    # Save machine-readable report
    report_json_path = results_dir / f"{model_name.lower().replace(' ', '_')}_classification_report.json"
    with open(report_json_path, "w", encoding="utf-8") as f:
        json.dump(report_dict, f, indent=2)

    # Save human-readable report
    report_txt_path = results_dir / f"{model_name.lower().replace(' ', '_')}_classification_report.txt"
    with open(report_txt_path, "w", encoding="utf-8") as f:
        f.write(report_text)

    # Confusion matrix CSV
    cm_df = pd.DataFrame(cm)
    cm_csv_path = results_dir / f"{model_name.lower().replace(' ', '_')}_confusion_matrix.csv"
    cm_df.to_csv(cm_csv_path, index=False)

    # Summary row
    summary_rows.append(
        {
            "model": model_name,
            "accuracy": report_dict.get("accuracy", None),
            "macro_f1": report_dict.get("macro avg", {}).get("f1-score", None),
            "weighted_f1": report_dict.get("weighted avg", {}).get("f1-score", None),
        }
    )

# ====================================
# ----- DECISION TREE CLASSIFIER -----
# ====================================
dtc = tree.DecisionTreeClassifier(random_state=1)
dtc.fit(X_train_balanced, y_train_balanced)
y_pred_dtc = dtc.predict(X_test)
evaluate_and_save("Decision Tree Classifier", y_test, y_pred_dtc)

# ====================================
# ----- RANDOM FOREST CLASSIFIER -----
# ====================================
rfc = RandomForestClassifier(
    n_estimators=100,
    max_depth=10,
    max_features="log2",
    random_state=1,
)
rfc.fit(X_train_balanced, y_train_balanced)
y_pred_rfc = rfc.predict(X_test)
evaluate_and_save("Random Forest Classifier", y_test, y_pred_rfc)

# ==========================
# ----- SVM CLASSIFIER -----
# ==========================
sv = svm.SVC(kernel="rbf", C=1.0, random_state=1)
sv.fit(X_train_scaled, y_train_balanced)
y_pred_svm = sv.predict(X_test_scaled)
evaluate_and_save("SVM Classifier", y_test, y_pred_svm)


# Save compact model comparison
summary_df = pd.DataFrame(summary_rows)
summary_df.to_csv(results_dir / "model_summary.csv", index=False)
print("\nSaved all model outputs in:", results_dir.resolve())

