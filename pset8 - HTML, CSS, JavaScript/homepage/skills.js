$(function() {
    // Array containing skills and its levels
    let skills = [
        {
            id: 'bi',
            name: 'Bubble.io',
            lvl: 15,
        },
        {
            id: 'py',
            name: 'Python',
            lvl: 20,
        },
        {
            id: 'css',
            name: 'CSS',
            lvl: 25,
        },
        {
            id: 'react',
            name: 'React',
            lvl: 35,
        },
        {
            id: 'js',
            name: 'Javascript',
            lvl: 40,
        },
        {
            id: 'reactn',
            name: 'React Native',
            lvl: 40,
        },
        {
            id: 'lx',
            name: 'Linux',
            lvl: 75,
        },
        {
            id: 'en',
            name: 'English',
            lvl: 80,
        }];

    // Crate a progress bar for each element of the previous array and change its id, title and progress level acordingly
    for (i = 0; i < skills.length; i++) {
        let template = $('#skills_template');
        let cloned = template.clone();
        cloned.attr('id', skills[i].id);
        cloned.attr("class", "row skill_bar");
        cloned.find("div.progress-bar").css("width", skills[i].lvl + "%");
        cloned.find("div.progress-bar").attr('aria-valuenow', skills[i].lvl);
        cloned.find("div.title").text(skills[i].name);
        // $(cloned.find("div.progress-bar")).animate({width: skills[i].lvl+"%"},2500);s
        cloned.appendTo("#skills_container");
    };
});