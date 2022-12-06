use std::collections::HashSet;
use std::fs;
use std::hash::Hash;

fn has_unique_elements<T>(iter: T) -> bool
where
    T: IntoIterator,
    T::Item: Eq + Hash,
{
    let mut uniq = HashSet::new();
    iter.into_iter().all(move |x| uniq.insert(x))
}

fn main() {
    let file_path = "inp_1.txt";
    println!("In file {}", file_path);

    let contents = fs::read_to_string(file_path).expect("Should have been able to read the file");

    println!("With text:\n{contents}");

    let pack_sz = 4;
    for i in 0..contents.len() - pack_sz {
        let slice = &contents[i..i + pack_sz];
        if has_unique_elements(slice.chars()) {
            println!("Part 1: {}", i + pack_sz);
            break;
        }
    }

    let pack_sz1 = 14;
    for i in 0..contents.len() - pack_sz1 {
        let slice = &contents[i..i + pack_sz1];
        if has_unique_elements(slice.chars()) {
            println!("Part 2: {}", i + pack_sz1);
            break;
        }
    }
}
