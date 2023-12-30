const link_variable = "https://www.lua.org/manual/5.3/manual.html#2.1";
const link_table = "https://www.lua.org/manual/5.3/manual.html#3.4.9";

function setTypeHtml(type, link) {
    type.innerHTML = `<a href=\"${link}\">&#60;${type.outerText}&#62;</a>`
}

function zebraChecklist(element) {
    let f = 0;
    for (let i = 0; i < element.length; ++i) {
        children = element[i].children;
        if (children) {
            for (let i = 0; i < children.length; ++i) {
                if (f % 2 == 0) {
                    children[i].querySelector("p").style.backgroundColor = "black";
                }
                let innerChild = children[i].querySelector("ul.checklist");
                if (innerChild) {
                    zebraChecklist(innerChild);
                }
                f++;
            }
        }
        f++;
    }
}

window.onload = function() {
    const class_api_type = document.querySelectorAll(".api-type");
    const class_checklist =   document.querySelectorAll("ul.checklist");
    const class_download =   document.querySelectorAll(".download");
    const id_footer =   document.querySelector("#footer");

    class_api_type.forEach(type => {
        switch(type.outerText) {
            case "number" : {
                setTypeHtml(type, link_variable);
                break;
            }
            case "string" : {
                setTypeHtml(type, link_variable);
                break;
            }
            case "table" : {
                setTypeHtml(type, link_table);
                break;
            }
        }
    });

    class_download.forEach(span => {
        let atag = span.querySelector("a");
        let href = atag.getAttribute("href");
        let regex_FileAtEndOfPath = /\/[\w-_]*.\w*$/;
        let fileName = href.match(regex_FileAtEndOfPath)[0].slice(1);
        atag.setAttribute("download", fileName);
        atag.innerHTML = fileName;
    })

    zebraChecklist(class_checklist);
    // footer.innerHTML += "<div>powered by asciidoctor</div>"
    // footer.querySelector("div").outHTML += "<div>powered by asciidoctor<div>";
    let footer = document.createElement("footer");
    footer.style.color = "black";
    footer.style.textAlign = "center";
    footer.style.backgroundColor = "white";
    footer.style.opacity = ".5"
    footer.innerHTML = "<p>powered by <a href='https://asciidoctor.org/'>asciidoctor</a></p>";
    document.body.appendChild(footer);
}
