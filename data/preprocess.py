import os

MUSHROOM_NAMES = ['class', 'cap-shape', 'cap-surface', 'cap-color', 'bruises', 'odor', 'gill-attachment',
                  'gill-spacing', 'gill-size', 'gill-color', 'stalk-shape', 'stalk-root',
                  'stalk-surface-above-ring', 'stalk-surface-below-ring', 'stalk-color-above-ring',
                  'stalk-color-below-ring', 'veil-type', 'veil-color', 'ring-number', 'ring-type',
                  'spore-print-color', 'population', 'habitat']

NURSERY_NAMES = ['parents', 'has-nurs', 'form', 'children', 'housing', 'finance', 'social', 'health', 'class']

CAR_NAMES = ['buying', 'maint', 'doors', 'persons', 'lug-boot', 'safety', 'class']


def preprocess(dataname, cols):
    with open('raw' + os.path.sep + dataname + '.data', 'r') as file:
        lines = file.readlines()
    lines = [l.rstrip().split(',') for l in lines]
    names = []
    for i in range(len(lines[0])):
        unique = []
        for j in range(len(lines)):
            if lines[j][i] not in unique:
                unique.append(lines[j][i])
            lines[j][i] = str(len(names) + unique.index(lines[j][i]))
        names.extend(['_'.join([cols[i], u]) for u in unique])
    with open('processed' + os.path.sep + dataname + '.data', 'w') as data_file:
        data_file.writelines([','.join(l) + '\n' for l in lines])
    with open('processed' + os.path.sep + dataname + '.names', 'w') as names_file:
        names_file.writelines([n + '\n' for n in names])


if __name__ == '__main__':
    preprocess('mushroom', MUSHROOM_NAMES)
    preprocess('nursery', NURSERY_NAMES)
    preprocess('car', CAR_NAMES)
